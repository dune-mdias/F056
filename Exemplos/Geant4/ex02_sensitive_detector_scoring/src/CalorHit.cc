#include "CalorHit.hh"

G4ThreadLocal G4Allocator<CalorHit>* CalorHitAllocator = nullptr;

CalorHit::CalorHit() : fLayerID(-1), fEdep(0.) {}
CalorHit::CalorHit(G4int layerID) : fLayerID(layerID), fEdep(0.) {}
CalorHit::~CalorHit() {}
