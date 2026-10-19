#!/usr/bin/env python3

import sys

import numpy as np
import ROOT
import matplotlib.pyplot as plt

from iminuit import Minuit
from scipy.special import erf


# ============================================================
# Configurações
# ============================================================

ARQUIVO_ROOT = "pythia_higgs.root"
NOME_ARVORE = "events"
NOME_RAMO = "mH"

MMIN = 50.0
MMAX = 400.0

NBINS = 1000


# ============================================================
# Configurações físicas para o peso dos eventos
# ============================================================

# Seção de choque em pb
sigma_pb = 1.0

# Luminosidade integrada em pb^-1
L = 100000.0

# Número total de eventos gerados na amostra.
#
# Se for None, o código usará len(dados).
# Caso o arquivo contenha somente uma seleção dos eventos
# gerados, informe aqui o número total original de eventos.
nEvents = None


# ============================================================
# Configurações do modelo
# ============================================================

# Pico fixo do background Breit-Wigner em GeV
BW_MU = 91.0


# ============================================================
# Leitura do arquivo ROOT
# ============================================================

def ler_dados_root(
    nome_arquivo,
    nome_arvore,
    nome_ramo
):

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
        arquivo.Close()

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

            arquivo.Close()

            raise RuntimeError(
                f"O ramo '{nome_ramo}' "
                f"não foi encontrado."
            )

    arquivo.Close()

    dados = np.asarray(
        dados,
        dtype=float
    )

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
# Sinal: distribuição gaussiana
# ============================================================

def normalizacao_gaussiana(
    mu,
    sigma
):
    """
    Integral da gaussiana no intervalo
    [MMIN, MMAX].
    """

    return 0.5 * (
        erf(
            (MMAX - mu)
            /
            (np.sqrt(2.0) * sigma)
        )
        -
        erf(
            (MMIN - mu)
            /
            (np.sqrt(2.0) * sigma)
        )
    )


def fs(
    x,
    mu,
    sigma
):
    """
    PDF do sinal gaussiano normalizada
    no intervalo [MMIN, MMAX].
    """

    normalizacao = normalizacao_gaussiana(
        mu,
        sigma
    )

    gaussiana = (
        np.exp(
            -0.5 * ((x - mu) / sigma)**2
        )
        /
        (
            np.sqrt(2.0 * np.pi)
            *
            sigma
        )
    )

    return gaussiana / normalizacao


# ============================================================
# Background 1: distribuição Breit-Wigner
# ============================================================

def breit_wigner_nao_normalizada(
    x,
    gamma
):
    """
    Distribuição de Breit-Wigner com pico
    fixo em BW_MU = 90 GeV.
    """

    return (
        1.0 / (2.0 * np.pi)
        *
        gamma
        /
        (
            (x - BW_MU)**2
            +
            gamma**2 / 4.0
        )
    )


def integral_breit_wigner(
    a,
    b,
    gamma
):
    """
    Integral analítica da Breit-Wigner
    no intervalo [a, b].
    """

    return (
        1.0 / np.pi
        *
        (
            np.arctan(
                2.0 * (b - BW_MU) / gamma
            )
            -
            np.arctan(
                2.0 * (a - BW_MU) / gamma
            )
        )
    )


def fbw(
    x,
    gamma
):
    """
    PDF do background Breit-Wigner
    normalizada no intervalo de análise.
    """

    normalizacao = integral_breit_wigner(
        MMIN,
        MMAX,
        gamma
    )

    return (
        breit_wigner_nao_normalizada(
            x,
            gamma
        )
        /
        normalizacao
    )


# ============================================================
# Background 2: exponencial decrescente
# ============================================================

def fexp(
    x,
    tau
):
    """
    PDF exponencial decrescente normalizada
    no intervalo [MMIN, MMAX].

    A forma usada é:

        exp(-(x - MMIN) / tau)
    """

    normalizacao = (
        tau
        *
        (
            1.0
            -
            np.exp(
                -(MMAX - MMIN) / tau
            )
        )
    )

    return (
        np.exp(
            -(x - MMIN) / tau
        )
        /
        normalizacao
    )


# ============================================================
# Máxima verossimilhança estendida
# ============================================================

def fazer_ajuste_ml(dados):

    def nll(
        mu,
        sigma,
        bw_gamma,
        tau,
        s,
        b_bw,
        b_exp
    ):

        sinal = fs(
            dados,
            mu,
            sigma
        )

        background_bw = fbw(
            dados,
            bw_gamma
        )

        background_exp = fexp(
            dados,
            tau
        )

        densidade = (
            s * sinal
            +
            b_bw * background_bw
            +
            b_exp * background_exp
        )

        densidade = np.maximum(
            densidade,
            1.e-300
        )

        rendimento_total = (
            s
            +
            b_bw
            +
            b_exp
        )

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
        mu=125.0,
        sigma=4.0,
        bw_gamma=5.0,
        tau=50.0,
        s=0.27 * n_total,
        b_bw=0.30 * n_total,
        b_exp=0.43 * n_total
    )

    fit.limits["mu"] = (
        MMIN,
        MMAX
    )

    fit.limits["sigma"] = (
        1.e-4,
        50.0
    )

    fit.limits["bw_gamma"] = (
        1.e-4,
        100.0
    )

    fit.limits["tau"] = (
        1.e-3,
        1000.0
    )

    fit.limits["s"] = (
        0.0,
        None
    )

    fit.limits["b_bw"] = (
        0.0,
        None
    )

    fit.limits["b_exp"] = (
        0.0,
        None
    )

    # Para -log L:
    # Delta(-log L) = 0.5 corresponde
    # aproximadamente a uma incerteza de 1 sigma.
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

    def chi2_fit(
        mu,
        sigma,
        bw_gamma,
        tau,
        s,
        b_bw,
        b_exp
    ):

        sinal = fs(
            centros,
            mu,
            sigma
        )

        background_bw = fbw(
            centros,
            bw_gamma
        )

        background_exp = fexp(
            centros,
            tau
        )

        previsao = (
            s * sinal
            +
            b_bw * background_bw
            +
            b_exp * background_exp
        ) * largura_bin

        variancia = np.maximum(
            contagens,
            1.0
        )

        residuos = (
            contagens
            -
            previsao
        )

        return np.sum(
            residuos**2
            /
            variancia
        )

    n_total = len(dados)

    fit = Minuit(
        chi2_fit,
        mu=92.0,
        sigma=4.0,
        bw_gamma=5.0,
        tau=50.0,
        s=0.27 * n_total,
        b_bw=0.30 * n_total,
        b_exp=0.43 * n_total
    )

    fit.limits["mu"] = (
        MMIN,
        MMAX
    )

    fit.limits["sigma"] = (
        1.e-4,
        50.0
    )

    fit.limits["bw_gamma"] = (
        1.e-4,
        100.0
    )

    fit.limits["tau"] = (
        1.e-3,
        1000.0
    )

    fit.limits["s"] = (
        0.0,
        None
    )

    fit.limits["b_bw"] = (
        0.0,
        None
    )

    fit.limits["b_exp"] = (
        0.0,
        None
    )

    # Para chi-quadrado:
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
        "sigma",
        "bw_gamma",
        "tau",
        "s",
        "b_bw",
        "b_exp"
    ]

    for parametro in parametros:

        valor = fit.values[parametro]
        erro = fit.errors[parametro]

        print(
            f"{parametro:>10s} = "
            f"{valor:.6f} +/- {erro:.6f}"
        )

    print()

    print(
        f"função mínima = {fit.fval:.6f}"
    )


# ============================================================
# Cálculo do peso dos eventos
# ============================================================

def imprimir_peso_eventos(
    n_selecionados
):

    if nEvents is None:

        n_gerados = n_selecionados

        print()
        print(
            "Aviso: nEvents não foi definido."
        )
        print(
            "Será usado o número de eventos selecionados "
            "como nGerados."
        )

    else:

        n_gerados = nEvents

    if n_gerados <= 0:

        raise RuntimeError(
            "O número de eventos gerados deve ser positivo."
        )

    eventWeightValue = (
        sigma_pb
        *
        L
        /
        n_gerados
    )

    eventos_esperados = (
        n_selecionados
        *
        eventWeightValue
    )

    print()
    print("=" * 70)
    print("Peso dos eventos")
    print("=" * 70)

    print(
        f"sigma_pb = {sigma_pb:.6f} pb"
    )

    print(
        f"L = {L:.6f} pb^-1"
    )

    print(
        f"nGerados = {n_gerados}"
    )

    print(
        f"peso = {eventWeightValue:.12g}"
    )

    print(
        f"eventos selecionados = {n_selecionados}"
    )

    print(
        f"eventos esperados = {eventos_esperados:.6f}"
    )

    return eventWeightValue, eventos_esperados


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

    # ========================================================
    # Modelo de máxima verossimilhança
    # ========================================================

    sinal_ml = (
        p_ml["s"]
        *
        fs(
            x,
            p_ml["mu"],
            p_ml["sigma"]
        )
        *
        largura_bin
    )

    background_bw_ml = (
        p_ml["b_bw"]
        *
        fbw(
            x,
            p_ml["bw_gamma"]
        )
        *
        largura_bin
    )

    background_exp_ml = (
        p_ml["b_exp"]
        *
        fexp(
            x,
            p_ml["tau"]
        )
        *
        largura_bin
    )

    total_ml = (
        sinal_ml
        +
        background_bw_ml
        +
        background_exp_ml
    )

    # ========================================================
    # Modelo de qui-quadrado
    # ========================================================

    sinal_chi2 = (
        p_chi2["s"]
        *
        fs(
            x,
            p_chi2["mu"],
            p_chi2["sigma"]
        )
        *
        largura_bin
    )

    background_bw_chi2 = (
        p_chi2["b_bw"]
        *
        fbw(
            x,
            p_chi2["bw_gamma"]
        )
        *
        largura_bin
    )

    background_exp_chi2 = (
        p_chi2["b_exp"]
        *
        fexp(
            x,
            p_chi2["tau"]
        )
        *
        largura_bin
    )

    total_chi2 = (
        sinal_chi2
        +
        background_bw_chi2
        +
        background_exp_chi2
    )

    # ========================================================
    # Construção do gráfico
    # ========================================================

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
        label="sinal gaussiano"
    )

    plt.plot(
        x,
        background_bw_ml,
        "m:",
        linewidth=2,
        label="background Breit-Wigner"
    )

    plt.plot(
        x,
        background_exp_ml,
        "c:",
        linewidth=2,
        label="background exponencial"
    )

    plt.xlabel(
        r"$m\;(\mathrm{GeV})$"
    )

    plt.ylabel(
        "eventos por bin"
    )

    plt.title(
        "Ajuste com sinal gaussiano e dois backgrounds"
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
        f"Número de eventos selecionados: {len(dados)}"
    )

    # Cálculo do peso dos eventos
    imprimir_peso_eventos(
        len(dados)
    )

    # Ajuste por máxima verossimilhança
    fit_ml = fazer_ajuste_ml(
        dados
    )

    # Ajuste por qui-quadrado
    fit_chi2, contagens, bordas = (
        fazer_ajuste_chi2(
            dados
        )
    )

    # Impressão dos resultados
    imprimir_resultados(
        "Ajuste por máxima verossimilhança",
        fit_ml
    )

    imprimir_resultados(
        "Ajuste por qui-quadrado",
        fit_chi2
    )

    # Gráfico
    fazer_grafico(
        dados,
        fit_ml,
        fit_chi2,
        contagens,
        bordas
    )


if __name__ == "__main__":

    main()

