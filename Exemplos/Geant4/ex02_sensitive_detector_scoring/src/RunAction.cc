#include "RunAction.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction() : G4UserRunAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);

    // Criacao de histogramas e Ntuple (salva em CSV ou ROOT automaticamente)
    analysisManager->CreateH1("EdepTotal", "Deposicao Total de Energia (MeV)", 100, 0., 1200.);

    analysisManager->CreateNtuple("CaloHits", "Deposicao por Evento");
    analysisManager->CreateNtupleDColumn("TotalEdep");
    analysisManager->FinishNtuple();
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("calo_output.csv");
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
