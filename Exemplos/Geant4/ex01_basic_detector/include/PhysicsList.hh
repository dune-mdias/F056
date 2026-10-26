#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

/// PhysicsList basica customizada herdando de G4VUserPhysicsList
class PhysicsList : public G4VUserPhysicsList {
public:
    PhysicsList();
    virtual ~PhysicsList();

protected:
    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;
    virtual void SetCuts() override;
};

#endif
