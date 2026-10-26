#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), fLogicTarget(nullptr) {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    // 1. Gerenciador de materiais do NIST
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* targetMat = nist->FindOrBuildMaterial("G4_Pb");

    // 2. Construcao do World Volume (1m x 1m x 1m)
    G4double worldSizeX = 1.0 * m;
    G4double worldSizeY = 1.0 * m;
    G4double worldSizeZ = 1.0 * m;

    G4Box* solidWorld = new G4Box("World", 0.5 * worldSizeX, 0.5 * worldSizeY, 0.5 * worldSizeZ);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(
        0,                  // sem rotacao
        G4ThreeVector(),    // na origem (0,0,0)
        logicWorld,         // logical volume
        "World",            // nome
        0,                  // mother volume (0 = world)
        false,              // no boolean operation
        0,                  // copy number
        true                // check overlaps
    );

    // 3. Construcao do Alvo de Chumbo (20cm x 20cm x 10cm)
    G4double targetX = 20.0 * cm;
    G4double targetY = 20.0 * cm;
    G4double targetZ = 10.0 * cm;

    G4Box* solidTarget = new G4Box("Target", 0.5 * targetX, 0.5 * targetY, 0.5 * targetZ);
    fLogicTarget = new G4LogicalVolume(solidTarget, targetMat, "Target");
    new G4PVPlacement(
        0,
        G4ThreeVector(0, 0, 0), // posicionado no centro
        fLogicTarget,
        "Target",
        logicWorld,
        false,
        0,
        true
    );

    // Atributos visuais (Alvo em cinza/azul transparente)
    G4VisAttributes* targetVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.8, 0.6));
    targetVisAtt->SetForceSolid(true);
    fLogicTarget->SetVisAttributes(targetVisAtt);
    logicWorld -> SetVisAttributes (G4VisAttributes::GetInvisible());
    return physWorld;
}
