#ifndef CalorHit_h
#define CalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

/// Class que representa o Hit de um deposito de energia no detector
class CalorHit : public G4VHit {
public:
    CalorHit();
    CalorHit(G4int layerID);
    virtual ~CalorHit();

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // Setters e Getters
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const  { return fEdep; }

    void SetLayerID(G4int id) { fLayerID = id; }
    G4int GetLayerID() const  { return fLayerID; }

private:
    G4int    fLayerID;
    G4double fEdep;
};

typedef G4THitsCollection<CalorHit> CalorHitsCollection;
extern G4ThreadLocal G4Allocator<CalorHit>* CalorHitAllocator;

inline void* CalorHit::operator new(size_t) {
    if(!CalorHitAllocator) CalorHitAllocator = new G4Allocator<CalorHit>;
    return (void *) CalorHitAllocator->MallocSingle();
}

inline void CalorHit::operator delete(void *hit) {
    CalorHitAllocator->FreeSingle((CalorHit*) hit);
}

#endif
