#ifndef CalorimeterSD_h
#define CalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"
#include "CalorHit.hh"

class G4Step;
class G4HCofThisEvent;

/// Sensitive Detector para o Calorimetro
class CalorimeterSD : public G4VSensitiveDetector {
public:
    CalorimeterSD(const G4String& name, const G4String& hitsCollectionName);
    virtual ~CalorimeterSD();

    virtual void   Initialize(G4HCofThisEvent* hitCollection) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection) override;

private:
    CalorHitsCollection* fHitsCollection;
};

#endif
