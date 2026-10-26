#ifndef PythiaPrimaryGeneratorAction_h
#define PythiaPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "Pythia8/Pythia.h"
#include "globals.hh"

class G4Event;

/// Gerador primario integrando o Pythia8 diretamente com o Geant4
class PythiaPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PythiaPrimaryGeneratorAction(const G4String& pythiaCard = "pythia_config.cmnd");
    virtual ~PythiaPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* anEvent) override;

private:
    Pythia8::Pythia fPythia;
};

#endif
