#include "ActionInitialization.hh"
#include "PythiaPrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

ActionInitialization::ActionInitialization(const G4String& pythiaCard)
: G4VUserActionInitialization(), fPythiaCard(pythiaCard) {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
    SetUserAction(new RunAction());
}

void ActionInitialization::Build() const {
    SetUserAction(new PythiaPrimaryGeneratorAction(fPythiaCard));
    SetUserAction(new RunAction());
    SetUserAction(new EventAction());
}
