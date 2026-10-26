#include "EventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"

EventAction::EventAction() : G4UserEventAction() {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*) {}

void EventAction::EndOfEventAction(const G4Event* event) {
    G4int nPrimaries = event->GetNumberOfPrimaryVertex();
    G4int totalParticles = 0;
    for (G4int i = 0; i < nPrimaries; ++i) {
        totalParticles += event->GetPrimaryVertex(i)->GetNumberOfParticle();
    }

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH1(0, totalParticles);
}
