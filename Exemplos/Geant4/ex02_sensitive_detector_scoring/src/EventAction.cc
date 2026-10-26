#include "EventAction.hh"
#include "CalorHit.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

EventAction::EventAction() : G4UserEventAction(), fHCID(-1) {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {}

void EventAction::EndOfEventAction(const G4Event* event) {
    if (fHCID == -1) {
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID("CalorHitsCollection");
    }

    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce) return;

    CalorHitsCollection* hc = (CalorHitsCollection*)(hce->GetHC(fHCID));
    if (!hc) return;

    G4double totalEdep = 0.0;
    for (size_t i = 0; i < hc->entries(); ++i) {
        totalEdep += (*hc)[i]->GetEdep();
    }

    // Preenche histograma e ntuple com a energia depositada no evento
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH1(0, totalEdep / MeV);

    analysisManager->FillNtupleDColumn(0, totalEdep / MeV);
    analysisManager->AddNtupleRow();
}
