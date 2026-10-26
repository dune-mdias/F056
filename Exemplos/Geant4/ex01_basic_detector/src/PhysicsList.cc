#include "PhysicsList.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4PhysicsListHelper.hh"
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() : G4VUserPhysicsList() {
    defaultCutValue = 1.0 * mm;
}

PhysicsList::~PhysicsList() {}

void PhysicsList::ConstructParticle() {
    // Registra particulas eletromagneticas basicas
    G4Gamma::GammaDefinition();
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();
}

void PhysicsList::ConstructProcess() {
    // Adiciona processo obrigatorio de transporte
    AddTransportation();

    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    auto particleIterator = GetParticleIterator();
    particleIterator->reset();

    while ((*particleIterator)()) {
        G4ParticleDefinition* particle = particleIterator->value();
        G4String particleName = particle->GetParticleName();

        if (particleName == "gamma") {
            ph->RegisterProcess(new G4PhotoElectricEffect, particle);
            ph->RegisterProcess(new G4ComptonScattering, particle);
            ph->RegisterProcess(new G4GammaConversion, particle);
        } else if (particleName == "e-") {
            ph->RegisterProcess(new G4eMultipleScattering, particle);
            ph->RegisterProcess(new G4eIonisation, particle);
            ph->RegisterProcess(new G4eBremsstrahlung, particle);
        } else if (particleName == "e+") {
            ph->RegisterProcess(new G4eMultipleScattering, particle);
            ph->RegisterProcess(new G4eIonisation, particle);
            ph->RegisterProcess(new G4eBremsstrahlung, particle);
            ph->RegisterProcess(new G4eplusAnnihilation, particle);
        }
    }
}

void PhysicsList::SetCuts() {
    SetCutsWithDefault();
}
