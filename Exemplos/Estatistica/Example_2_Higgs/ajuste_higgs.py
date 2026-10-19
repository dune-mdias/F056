#!/usr/bin/env python3

import sys
import numpy as np
import ROOT
import matplotlib.pyplot as plt

from iminuit import Minuit
from scipy.special import gamma as gamma_function


# ============================================================
# Configurações
# ============================================================

ARQUIVO_ROOT = "pythia_higgs.root"
NOME_ARVORE = "events"
NOME_RAMO = "mH"

MMIN = 0.0
MMAX = 400.0

NBINS = 1000


# ============================================================
# Leitura do arquivo ROOT
# ============================================================

def ler_dados_root(nome_arquivo,
                   nome_arvore,
                   nome_ramo):

    arquivo = ROOT.TFile.Open(
        nome_arquivo,
        "READ"
    )

    if not arquivo or arquivo.IsZombie():
        raise RuntimeError(
            f"Não foi possível abrir o arquivo "
            f"{nome_arquivo}"
        )

    arvore = arquivo.Get(nome_arvore)

    if not arvore:
        raise RuntimeError(
            f"A árvore '{nome_arvore}' "
            f"não foi encontrada."
        )

    dados = []

    for evento in arvore:
        try:
            valor = float(
                getattr(evento, nome_ramo)
            )
            dados.append(valor)

        except AttributeError:
            raise RuntimeError(
                f"O ramo '{nome_ramo}' "
                f"não foi encontrado."
            )

    arquivo.Close()

    dados = np.asarray(dados)

    # Seleção da região de análise
    dados = dados[
        (dados >= MMIN)
        &
        (dados <= MMAX)
    ]

    if len(dados) == 0:
        raise RuntimeError(
            "Nenhum evento foi encontrado "
            "no intervalo selecionado."
        )

    return dados


# ============================================================
# Sinal: distribuição de Breit-Wigner
# ============================================================

def breit_wigner_nao_normalizada(
    x,
    mu,
    gamma
):

    return (
        1.0 / (2.0 * np.pi)
        *
        gamma
        /
        (
            (x - mu)**2
            +
            gamma**2 / 4.0
        )
    )


def integral_breit_wigner(
    a,
    b,
    mu,
    gamma
):

    return (
        1.0 / np.pi
        *
        (
            np.arctan(
                2.0 * (b - mu) / gamma
            )
            -
            np.arctan(
                2.0 * (a - mu) / gamma
            )
        )
    )


def fs(x, mu, gamma):
    """
    PDF do sinal normalizada no intervalo
    [MMIN, MMAX].
    """

    normalizacao = integral_breit_wigner(
        MMIN,
        MMAX,
        mu,
        gamma
    )

    return (
        breit_wigner_nao_normalizada(
            x,
            mu,
            gamma
        )
        /
        normalizacao
    )


# ============================================================
# Background: distribuição qui-quadrado
# ============================================================

def chi2_background_nao_normalizado(
    x,
    dof,
    scale
):
    """
    Distribuição qui-quadrado deslocada e escalada.

    y = (m - MMIN) / scale

    f(y) = y^(nu/2 - 1) exp(-y/2)
           / [2^(nu/2) Gamma(nu/2)]
    """

    y = (x - MMIN) / scale

    y = np.maximum(
        y,
        1.e-12
    )

    pdf_y = (
        y**(dof / 2.0 - 1.0)
        *
        np.exp(-y / 2.0)
        /
        (
            2.0**(dof / 2.0)
            *
            gamma_function(dof / 2.0)
        )
    )

    # Transformação de variável:
    # f_m(m) = f_y(y) / scale
    return pdf_y / scale


def normalizacao_background(
    dof,
    scale
):
    """
    Normalização numérica do background
    no intervalo [MMIN, MMAX].
    """

    x = np.linspace(
        MMIN,
        MMAX,
        3000
    )

    valores = chi2_background_nao_normalizado(
        x,
        dof,
        scale
    )

    integral = np.trapz(
        valores,
        x
    )

    return integral


def fb(x, dof, scale):
    """
    PDF do background qui-quadrado,
    normalizada no intervalo de análise.
    """

    normalizacao = normalizacao_background(
        dof,
        scale
    )

    return (
        chi2_background_nao_normalizado(
            x,
            dof,
            scale
        )
        /
        normalizacao
    )


# ============================================================
# Máxima verossimilhança estendida
# ============================================================

def fazer_ajuste_ml(dados):

    def nll(mu, gamma, dof, scale, s, b):

        sinal = fs(
            dados,
            mu,
            gamma
        )

        background = fb(
            dados,
            dof,
            scale
        )

        densidade = (
            s * sinal
            +
            b * background
        )

        densidade = np.maximum(
            densidade,
            1.e-300
        )

        rendimento_total = s + b

        return (
            rendimento_total
            -
            np.sum(
                np.log(densidade)
            )
        )

    n_total = len(dados)

    fit = Minuit(
        nll,
        mu=92.0,
        gamma=4.0,
        dof=3.0,
        scale=0.20,
        #s=2700.0,
        s=0.27 * n_total,
        b=0.73 * n_total
    )

    fit.limits["mu"] = (
        MMIN,
        MMAX
    )

    fit.limits["gamma"] = (
        1.e-4,
        10.0
    )

    fit.limits["dof"] = (
        0.2,
        40.0
    )

    fit.limits["scale"] = (
        1.e-3,
        19.0
    )

    fit.limits["s"] = (
        0.0,
        None
    )

    fit.limits["b"] = (
        0.0,
        None
    )

    # Para -log L:
    # Delta(-log L) = 0.5 corresponde aproximadamente
    # a uma incerteza de 1 sigma.
    fit.errordef = Minuit.LIKELIHOOD

    fit.migrad()

    if not fit.valid:
        print(
            "Aviso: o ajuste de máxima "
            "verossimilhança não foi considerado válido."
        )

    fit.hesse()

    return fit


# ============================================================
# Ajuste de qui-quadrado dos dados binados
# ============================================================

def fazer_ajuste_chi2(dados):

    contagens, bordas = np.histogram(
        dados,
        bins=NBINS,
        range=(MMIN, MMAX)
    )

    centros = 0.5 * (
        bordas[1:]
        +
        bordas[:-1]
    )

    largura_bin = (
        bordas[1]
        -
        bordas[0]
    )

    def chi2_fit(mu, gamma, dof, scale, s, b):

        sinal = fs(
            centros,
            mu,
            gamma
        )

        background = fb(
            centros,
            dof,
            scale
        )

        previsao = (
            s * sinal
            +
            b * background
        ) * largura_bin

        variancia = np.maximum(
            contagens,
            1.0
        )

        residuos = contagens - previsao

        return np.sum(
            residuos**2
            /
            variancia
        )

    n_total = len(dados)

    fit = Minuit(
        chi2_fit,
        mu=92.0,
        gamma=4.0,
        dof=6.0,
        scale=13,
        #s=2700.0,
        s=0.27 * n_total,
        b=0.8 * n_total
    )

    fit.limits["mu"] = (
        MMIN,
        MMAX
    )

    fit.limits["gamma"] = (
        1.e-4,
        10.0
    )

    fit.limits["dof"] = (
        0.2,
        40.0
    )

    fit.limits["scale"] = (
        1.e-3,
        19.0
    )

    fit.limits["s"] = (
        0.1,
        None
    )

    fit.limits["b"] = (
        0.7 ,
        None
    )

    # Para chi2:
    # Delta chi2 = 1 corresponde aproximadamente
    # a uma incerteza de 1 sigma.
    fit.errordef = Minuit.LEAST_SQUARES

    fit.migrad()

    if not fit.valid:
        print(
            "Aviso: o ajuste de qui-quadrado "
            "não foi considerado válido."
        )

    fit.hesse()

    return fit, contagens, bordas


# ============================================================
# Impressão dos resultados
# ============================================================

def imprimir_resultados(
    titulo,
    fit
):

    print()
    print("=" * 70)
    print(titulo)
    print("=" * 70)

    parametros = [
        "mu",
        "gamma",
        "dof",
        "scale",
        "s",
        "b"
    ]

    for parametro in parametros:

        valor = fit.values[parametro]
        erro = fit.errors[parametro]

        print(
            f"{parametro:>8s} = "
            f"{valor:.6f} +/- {erro:.6f}"
        )

    print()
    print(
        f"função mínima = {fit.fval:.6f}"
    )


# ============================================================
# Gráfico
# ============================================================

def fazer_grafico(
    dados,
    fit_ml,
    fit_chi2,
    contagens,
    bordas
):

    centros = 0.5 * (
        bordas[1:]
        +
        bordas[:-1]
    )

    largura_bin = (
        bordas[1]
        -
        bordas[0]
    )

    erros = np.sqrt(
        np.maximum(
            contagens,
            1.0
        )
    )

    x = np.linspace(
        MMIN,
        MMAX,
        1500
    )

    p_ml = fit_ml.values
    p_chi2 = fit_chi2.values

    # Modelo de máxima verossimilhança
    sinal_ml = (
        p_ml["s"]
        *
        fs(
            x,
            p_ml["mu"],
            p_ml["gamma"]
        )
        *
        largura_bin
    )

    background_ml = (
        p_ml["b"]
        *
        fb(
            x,
            p_ml["dof"],
            p_ml["scale"]
        )
        *
        largura_bin
    )

    total_ml = (
        sinal_ml
        +
        background_ml
    )

    # Modelo de qui-quadrado
    total_chi2 = (
        p_chi2["s"]
        *
        fs(
            x,
            p_chi2["mu"],
            p_chi2["gamma"]
        )
        *
        largura_bin
        +
        p_chi2["b"]
        *
        fb(
            x,
            p_chi2["dof"],
            p_chi2["scale"]
        )
        *
        largura_bin
    )

    plt.figure(
        figsize=(10, 6)
    )

    plt.errorbar(
        centros,
        contagens,
        yerr=erros,
        fmt="ko",
        markersize=3,
        label="dados"
    )

    plt.plot(
        x,
        total_ml,
        "b-",
        linewidth=2,
        label="máxima verossimilhança"
    )

    plt.plot(
        x,
        total_chi2,
        "r--",
        linewidth=2,
        label=r"ajuste $\chi^2$"
    )

    plt.plot(
        x,
        sinal_ml,
        "g:",
        linewidth=2,
        label="sinal Breit-Wigner"
    )

    plt.plot(
        x,
        background_ml,
        "m:",
        linewidth=2,
        label=r"background $\chi^2$"
    )

    plt.xlabel(
        r"$m\;(\mathrm{GeV})$"
    )

    plt.ylabel(
        "eventos por bin"
    )

    plt.title(
        "Ajuste com background qui-quadrado"
    )

    plt.legend()
    plt.grid(alpha=0.25)
    plt.tight_layout()
    plt.show()


# ============================================================
# Programa principal
# ============================================================

def main():

    nome_arquivo = ARQUIVO_ROOT

    if len(sys.argv) > 1:
        nome_arquivo = sys.argv[1]

    dados = ler_dados_root(
        nome_arquivo,
        NOME_ARVORE,
        NOME_RAMO
    )

    print(
        f"Arquivo utilizado: {nome_arquivo}"
    )

    print(
        f"Número de eventos: {len(dados)}"
    )

    fit_ml = fazer_ajuste_ml(
        dados
    )

    fit_chi2, contagens, bordas = (
        fazer_ajuste_chi2(dados)
    )

    imprimir_resultados(
        "Ajuste por máxima verossimilhança",
        fit_ml
    )

    imprimir_resultados(
        "Ajuste por qui-quadrado",
        fit_chi2
    )

    fazer_grafico(
        dados,
        fit_ml,
        fit_chi2,
        contagens,
        bordas
    )


if __name__ == "__main__":
    main()

