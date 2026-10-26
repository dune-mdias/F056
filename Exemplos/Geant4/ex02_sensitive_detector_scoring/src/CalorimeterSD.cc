#include "CalorimeterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

CalorimeterSD::CalorimeterSD(const G4String& name, const G4String& hitsCollectionName)
: G4VSensitiveDetector(name), fHitsCollection(nullptr) {
    collectionName.insert(hitsCollectionName);
}

CalorimeterSD::~CalorimeterSD() {}

void CalorimeterSD::Initialize(G4HCofThisEvent* hce) {
    fHitsCollection = new CalorHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool CalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep == 0.) return false;

    G4int copyNo = step->GetPreStepPoint()->GetTouchable()->GetCopyNumber();

    // Procura se ja existe um Hit registrado para esta camada
    CalorHit* hit = nullptr;
    for (size_t i = 0; i < fHitsCollection->entries(); ++i) {
        if ((*fHitsCollection)[i]->GetLayerID() == copyNo) {
            hit = (*fHitsCollection)[i];
            break;
        }
    }

    if (!hit) {
        hit = new CalorHit(copyNo);
        fHitsCollection->insert(hit);
    }

    hit->AddEdep(edep);
    return true;
}

void CalorimeterSD::EndOfEvent(G4HCofThisEvent*) {}
