"""
distribuicoes.py

Gera amostras das distribuicoes classicas (Gaussiana, Poisson, Binomial,
Exponencial e Qui-quadrado) usando NumPy, e sobrepoe a curva teorica
(PDF ou PMF) calculada com SciPy, para visualizar o quanto o histograma
de uma amostra finita se aproxima da distribuicao teorica.

Rodar com:
    python3 distribuicoes.py

Gera um arquivo distribuicoes.png com um painel 2x3.
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# Semente fixa para reprodutibilidade dos resultados no material didatico
rng = np.random.default_rng(seed=42)

N_AMOSTRAS = 20_000


def plot_continua(ax, amostra, x_min, x_max, pdf_teorica, titulo):
    """Histograma normalizado (densidade) + curva teorica para variavel continua."""
    ax.hist(amostra, bins=60, density=True, alpha=0.6,
            color="steelblue", edgecolor="white", label="amostra (NumPy)")
    x = np.linspace(x_min, x_max, 400)
    ax.plot(x, pdf_teorica(x), "r-", lw=2, label="PDF teorica (SciPy)")
    ax.set_title(titulo)
    ax.legend(fontsize=8)


def plot_discreta(ax, amostra, k_max, pmf_teorica, titulo):
    """Histograma de contagens + PMF teorica para variavel discreta."""
    k = np.arange(0, k_max + 1)
    bins = np.arange(-0.5, k_max + 1.5, 1)
    ax.hist(amostra, bins=bins, density=True, alpha=0.6,
            color="darkorange", edgecolor="white", label="amostra (NumPy)")
    ax.plot(k, pmf_teorica(k), "ko-", ms=4, lw=1, label="PMF teorica (SciPy)")
    ax.set_title(titulo)
    ax.legend(fontsize=8)


def main():
    fig, axes = plt.subplots(2, 3, figsize=(15, 9))

    # 1) Gaussiana / Normal ------------------------------------------------
    mu, sigma = 0.0, 1.0
    amostra_gauss = rng.normal(mu, sigma, N_AMOSTRAS)
    plot_continua(
        axes[0, 0], amostra_gauss, -4, 4,
        lambda x: stats.norm.pdf(x, mu, sigma),
        f"Gaussiana(mu={mu}, sigma={sigma})",
    )

    # 2) Exponencial ---------------------------------------------------------
    taxa = 1.5  # lambda; NumPy parametriza por "scale" = 1/lambda
    amostra_exp = rng.exponential(scale=1 / taxa, size=N_AMOSTRAS)
    plot_continua(
        axes[0, 1], amostra_exp, 0, 5,
        lambda x: stats.expon.pdf(x, scale=1 / taxa),
        f"Exponencial(lambda={taxa})",
    )

    # 3) Qui-quadrado ----------------------------------------------------
    graus_liberdade = 4
    amostra_chi2 = rng.chisquare(graus_liberdade, N_AMOSTRAS)
    plot_continua(
        axes[0, 2], amostra_chi2, 0, 20,
        lambda x: stats.chi2.pdf(x, graus_liberdade),
        f"Qui-quadrado (k={graus_liberdade} g.l.)",
    )

    # 4) Poisson -------------------------------------------------------------
    lam = 4.0
    amostra_poisson = rng.poisson(lam, N_AMOSTRAS)
    plot_discreta(
        axes[1, 0], amostra_poisson, k_max=15,
        pmf_teorica=lambda k: stats.poisson.pmf(k, lam),
        titulo=f"Poisson(lambda={lam})",
    )

    # 5) Binomial --------------------------------------------------------
    n_tentativas, p_sucesso = 20, 0.3
    amostra_binom = rng.binomial(n_tentativas, p_sucesso, N_AMOSTRAS)
    plot_discreta(
        axes[1, 1], amostra_binom, k_max=n_tentativas,
        pmf_teorica=lambda k: stats.binom.pmf(k, n_tentativas, p_sucesso),
        titulo=f"Binomial(n={n_tentativas}, p={p_sucesso})",
    )

    # Painel extra: Poisson como limite da Binomial quando n grande e p pequeno
    n_grande, p_pequeno = 2000, lam / 2000
    amostra_binom_lim = rng.binomial(n_grande, p_pequeno, N_AMOSTRAS)
    plot_discreta(
        axes[1, 2], amostra_binom_lim, k_max=15,
        pmf_teorica=lambda k: stats.poisson.pmf(k, lam),
        titulo=f"Binomial(n={n_grande}, p={p_pequeno:.4f}) ~ Poisson({lam})",
    )

    fig.suptitle("Distribuicoes classicas: amostra (NumPy) vs. teoria (SciPy)", fontsize=14)
    fig.tight_layout(rect=[0, 0, 1, 0.96])
    fig.savefig("distribuicoes.png", dpi=130)
    print("Figura salva em distribuicoes.png")

    # Impressao numerica de media/variancia amostral vs teorica (Gaussiana)
    print(f"\nGaussiana: media amostral={amostra_gauss.mean():.4f} "
          f"(teorica={mu}), var amostral={amostra_gauss.var():.4f} "
          f"(teorica={sigma**2})")


if __name__ == "__main__":
    main()
