#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
    // Acções executadas apenas pela thread mestre (se houver RunAction global)
}

void ActionInitialization::Build() const {
    // Registra as acoes de usuario para cada worker thread
    SetUserAction(new PrimaryGeneratorAction());
}
