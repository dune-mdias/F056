#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* airMat  = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* siMat   = nist->FindOrBuildMaterial("G4_Si");
    G4Material* pbMat   = nist->FindOrBuildMaterial("G4_Pb");

    // World Volume
    G4Box* solidWorld = new G4Box("World", 2.0*m, 2.0*m, 3.0*m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, airMat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

    // Tracker Cilíndrico de Silício (rIn = 20cm, rOut = 21cm, z = 2m)
    G4Tubs* solidTracker = new G4Tubs("Tracker", 20.*cm, 21.*cm, 1.0*m, 0.*deg, 360.*deg);
    G4LogicalVolume* logicTracker = new G4LogicalVolume(solidTracker, siMat, "Tracker");
    new G4PVPlacement(0, G4ThreeVector(), logicTracker, "Tracker", logicWorld, false, 0, true);

    // Calorímetro EM Cilíndrico de Chumbo (rIn = 50cm, rOut = 80cm, z = 2.5m)
    G4Tubs* solidCalo = new G4Tubs("Calorimeter", 50.*cm, 80.*cm, 1.25*m, 0.*deg, 360.*deg);
    G4LogicalVolume* logicCalo = new G4LogicalVolume(solidCalo, pbMat, "Calorimeter");
    new G4PVPlacement(0, G4ThreeVector(), logicCalo, "Calorimeter", logicWorld, false, 0, true);

    // Atributos Visuais
    G4VisAttributes* trackVis = new G4VisAttributes(G4Colour(0.0, 0.5, 1.0, 0.6));
    trackVis->SetForceSolid(true);
    logicTracker->SetVisAttributes(trackVis);

    G4VisAttributes* caloVis = new G4VisAttributes(G4Colour(0.9, 0.7, 0.0, 0.4));
    caloVis->SetForceSolid(true);
    logicCalo->SetVisAttributes(caloVis);

    return physWorld;
}
