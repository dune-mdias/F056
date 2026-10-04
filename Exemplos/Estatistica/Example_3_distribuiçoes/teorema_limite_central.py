"""
teorema_limite_central.py

Demonstra o Teorema Central do Limite (TCL): soma de n variaveis
aleatorias uniformes (claramente NAO gaussianas) se aproxima de uma
Gaussiana conforme n cresce.

Rodar com:
    python3 teorema_limite_central.py

Gera tcl_convergencia.png com um painel para diferentes valores de n.
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

rng = np.random.default_rng(seed=1)

N_REPETICOES = 50_000          # quantas somas independentes simulamos
VALORES_DE_N = [1, 2, 5, 30]   # numero de variaveis uniformes somadas em cada caso

# Uniforme(0,1): media teorica = 1/2, variancia teorica = 1/12
MEDIA_UNIF, VAR_UNIF = 0.5, 1.0 / 12.0


def soma_padronizada(n):
    """
    Soma n variaveis Uniforme(0,1) independentes, repetida N_REPETICOES vezes,
    e padroniza o resultado (subtrai a media teorica da soma, divide pelo
    desvio padrao teorico da soma) para que a comparacao com N(0,1) seja direta.
    """
    amostras = rng.uniform(0, 1, size=(N_REPETICOES, n))
    somas = amostras.sum(axis=1)

    media_soma = n * MEDIA_UNIF
    desvio_soma = np.sqrt(n * VAR_UNIF)

    return (somas - media_soma) / desvio_soma


def main():
    fig, axes = plt.subplots(1, len(VALORES_DE_N), figsize=(4 * len(VALORES_DE_N), 4), sharey=True)

    x = np.linspace(-4, 4, 400)
    pdf_normal_padrao = stats.norm.pdf(x)

    for ax, n in zip(axes, VALORES_DE_N):
        z = soma_padronizada(n)
        ax.hist(z, bins=60, density=True, alpha=0.6, color="mediumseagreen",
                 edgecolor="white", label=f"soma de n={n}\nvariaveis U(0,1)\n(padronizada)")
        ax.plot(x, pdf_normal_padrao, "r-", lw=2, label="N(0,1)")
        ax.set_title(f"n = {n}")
        ax.set_xlabel("z")
        if n == VALORES_DE_N[0]:
            ax.set_ylabel("densidade")
        ax.legend(fontsize=7, loc="upper right")

    fig.suptitle(
        "Teorema Central do Limite: soma de variaveis Uniforme(0,1) padronizada, "
        "convergindo para N(0,1)",
        fontsize=12,
    )
    fig.tight_layout(rect=[0, 0, 1, 0.93])
    fig.savefig("tcl_convergencia.png", dpi=130)
    print("Figura salva em tcl_convergencia.png")

    # Quantifica a convergencia com um teste de normalidade simples (Shapiro-Wilk
    # em uma sub-amostra, pois o teste completo com 50000 pontos e custoso)
    print("\nTeste de normalidade (Shapiro-Wilk, sub-amostra de 500 pontos):")
    for n in VALORES_DE_N:
        z = soma_padronizada(n)
        sub = rng.choice(z, size=500, replace=False)
        estat, p_valor = stats.shapiro(sub)
        print(f"  n={n:>3d}: estatistica W={estat:.4f}, p-value={p_valor:.4g}"
              f"  {'(compativel com Gaussiana)' if p_valor > 0.05 else '(ainda destoa da Gaussiana)'}")


if __name__ == "__main__":
    main()
