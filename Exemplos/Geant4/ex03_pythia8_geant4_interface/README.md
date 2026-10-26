# Exemplo 03: Interface Pythia8 + Geant4 (ex03_pythia8_geant4_interface)

Este exemplo demonstra a integração direta do motor de geração **Pythia8** dentro do Geant4:
- O Pythia8 gera colisões $pp \to Z^0/\gamma^* \to e^+ e^-$ a $\sqrt{s} = 13\text{ TeV}$.
- As partículas estáveis do estado final são injetadas como primárias no Geant4.
- As partículas atravessam um Tracker de Silício e um Calorímetro EM de Chumbo.
- A multiplicidade por evento é salva em `pythia_g4_output.csv`.

## Requisitos
- **Geant4** instalado.
- **Pythia8** instalado (biblioteca `libpythia8.so` e cabeçalho `Pythia8/Pythia.h`).

## Como Compilar e Rodar

```bash
mkdir build
cd build
cmake ..
make -j4
./ex03_pythia8_geant4_interface run.mac
```
