import pandas as pd, matplotlib.pyplot as plt, numpy as np
from scipy.optimize import curve_fit

# Le histograma H1 gerado pelo G4AnalysisManager
h = pd.read_csv('calo_output_h1_EdepTotal.csv', comment='#',
                header=None, names=['E_MeV','counts'])
h = h[pd.to_numeric(h['E_MeV'], errors='coerce').notna()].astype(float)

# Ajuste gaussiano para extrair resolucao de energia
def gauss(x, N, mu, sigma):
    return N * np.exp(-0.5*((x-mu)/sigma)**2)

E, n = h['E_MeV'].values, h['counts'].values
popt, _ = curve_fit(gauss, E, n, p0=[n.max(), E[n.argmax()], 20.])
mu, sigma = popt[1], abs(popt[2])
print(f'<E> = {mu:.1f} MeV,  sigma/E = {sigma/mu*100:.1f}%')

plt.bar(E, n, width=(E[1]-E[0])*0.9, alpha=0.6, label='Geant4')
plt.plot(E, gauss(E, *popt), 'r-', label=fr'Gaussiana: $\sigma/E={sigma/mu*100:.1f}\%$')
plt.xlabel('Energia Depositada (MeV)'); plt.legend(); plt.savefig('histo_edep.pdf')
