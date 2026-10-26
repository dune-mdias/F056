#include "PythiaPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

PythiaPrimaryGeneratorAction::PythiaPrimaryGeneratorAction(const G4String& pythiaCard)
: G4VUserPrimaryGeneratorAction() {
    // Configura o Pythia8 lendo o cartao de parametros
    fPythia.readFile(pythiaCard);
    fPythia.init();
}

PythiaPrimaryGeneratorAction::~PythiaPrimaryGeneratorAction() {}

void PythiaPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    if (!fPythia.next()) return;

    // Cria o vertice primario na origem (0,0,0)
    G4PrimaryVertex* vertex = new G4PrimaryVertex(0.*cm, 0.*cm, 0.*cm, 0.*ns);

    // Percorre todas as particulas geradas pelo Pythia8 no evento
    for (int i = 0; i < fPythia.event.size(); ++i) {
        // Apenas particulas finais estaveis sao injetadas no Geant4
        if (fPythia.event[i].isFinal()) {
            G4int pdgCode = fPythia.event[i].id();
            G4double px = fPythia.event[i].px() * GeV;
            G4double py = fPythia.event[i].py() * GeV;
            G4double pz = fPythia.event[i].pz() * GeV;

            G4PrimaryParticle* particle = new G4PrimaryParticle(pdgCode, px, py, pz);
            vertex->SetPrimary(particle);
        }
    }

    anEvent->AddPrimaryVertex(vertex);
}
