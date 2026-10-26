#include "DetectorConstruction.hh"
#include "CalorimeterSD.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), fLogicScint(nullptr) {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* airMat   = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* leadMat  = nist->FindOrBuildMaterial("G4_Pb");

    // Tenta encontrar o material de cintilador no NIST ou cria um fallback robusto
    G4Material* scintMat = nist->FindOrBuildMaterial("G4_PLASTIC_SCINTILLATOR");
    if (!scintMat) {
        scintMat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
    }
    if (!scintMat) {
        // Fallback: constroi o material de Cintilador Plastico (C10H11, densidade 1.032 g/cm3)
        G4double density = 1.032 * g/cm3;
        scintMat = new G4Material("PlasticScintillator", density, 2);
        G4Element* C = nist->FindOrBuildElement("C");
        G4Element* H = nist->FindOrBuildElement("H");
        scintMat->AddElement(C, 10);
        scintMat->AddElement(H, 11);
    }

    // World Volume
    G4Box* solidWorld = new G4Box("World", 0.5*m, 0.5*m, 1.0*m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, airMat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

    // Calorimetro Sanduiche: 10 Camadas de (5mm Pb + 10mm Cintilador)
    G4int nLayers = 10;
    G4double pbThick = 5.0 * mm;
    G4double scintThick = 10.0 * mm;
    G4double layerThick = pbThick + scintThick;
    G4double caloWidth = 20.0 * cm;

    G4double startZ = -0.5 * nLayers * layerThick;

    G4Box* solidPb = new G4Box("PbAbsorber", 0.5*caloWidth, 0.5*caloWidth, 0.5*pbThick);
    G4LogicalVolume* logicPb = new G4LogicalVolume(solidPb, leadMat, "PbAbsorber");

    G4Box* solidScint = new G4Box("ScintLayer", 0.5*caloWidth, 0.5*caloWidth, 0.5*scintThick);
    fLogicScint = new G4LogicalVolume(solidScint, scintMat, "ScintLayer");

    for (G4int i = 0; i < nLayers; ++i) {
        G4double zPb = startZ + i * layerThick + 0.5 * pbThick;
        G4double zScint = zPb + 0.5 * pbThick + 0.5 * scintThick;

        new G4PVPlacement(0, G4ThreeVector(0, 0, zPb), logicPb, "PbAbsorber", logicWorld, false, i, true);
        new G4PVPlacement(0, G4ThreeVector(0, 0, zScint), fLogicScint, "ScintLayer", logicWorld, false, i, true);
    }

    // Cores visuais
    G4VisAttributes* pbVis = new G4VisAttributes(G4Colour(0.4, 0.4, 0.4, 0.5));
    pbVis->SetForceSolid(true);
    logicPb->SetVisAttributes(pbVis);

    G4VisAttributes* scintVis = new G4VisAttributes(G4Colour(0.0, 0.8, 0.2, 0.7));
    scintVis->SetForceSolid(true);
    fLogicScint->SetVisAttributes(scintVis);

    return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
    // Instancia o Sensitive Detector e associa ao Logical Volume do Cintilador
    CalorimeterSD* caloSD = new CalorimeterSD("CalorimeterSD", "CalorHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(caloSD);
    SetSensitiveDetector(fLogicScint, caloSD);
}
