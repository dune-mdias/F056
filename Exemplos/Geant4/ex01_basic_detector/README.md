# Exemplo 01: Geometria Básica e Canhão de Partículas (ex01_basic_detector)

Este exemplo demonstra a aplicação Geant4 minimal contendo:
- World Volume de Ar.
- Alvo de Chumbo (`G4_Pb`) no centro.
- Canhão de Partículas (`G4ParticleGun`) disparando elétrons de $500\text{ MeV}$.
- Lista de Física Eletromagnética básica (`PhysicsList`).

## Como Compilar e Rodar

### Opção A: Usando CMake (Recomendado)
```bash
mkdir build
cd build
cmake ..
make -j4
./ex01_basic_detector           # Modo interativo (GUI OpenGL)
./ex01_basic_detector run.mac   # Modo em lote (Batch mode)
```

### Opção B: Usando Makefile
```bash
make
./ex01_basic_detector
```
