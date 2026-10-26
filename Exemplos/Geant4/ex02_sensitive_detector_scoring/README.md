# Exemplo 02: Sensitive Detector, Hits e Exportação de Dados (ex02_sensitive_detector_scoring)

Este exemplo demonstra:
- Calorímetro amostrador tipo Sanduíche (Chumbo + Plástico Cintilador).
- Registro de Hits por camada sensível com `G4VSensitiveDetector` (`CalorimeterSD`).
- Ações de Usuário (`RunAction` e `EventAction`) com `G4AnalysisManager` gerando saída em CSV/ROOT (`calo_output.csv`).
- Lista de Física padrão `FTFP_BERT`.

## Como Compilar e Rodar

```bash
mkdir build
cd build
cmake ..
make -j4
./ex02_sensitive_detector_scoring run.mac
```
Ao final da execução, o arquivo `calo_output.csv` conterá a energia total depositada por evento.
