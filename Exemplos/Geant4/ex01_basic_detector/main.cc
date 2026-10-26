#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv) {
    // 1. Detecta se a execucao eh interativa (GUI) ou via macro (batch)
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // 2. Instancia o RunManager: SerialOnly para GUI (evita erro de thread do Qt OpenGL), ou MT para Batch
    auto runManagerType = ui ? G4RunManagerType::SerialOnly : G4RunManagerType::Default;
    auto* runManager = G4RunManagerFactory::CreateRunManager(runManagerType);

    // 3. Registra as classes de inicializacao do usuario
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    // 4. Inicializa o gerenciador de visualizacao 3D (OpenGL/Qt)
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // 5. Gerenciador de Comandos de UI
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        // Modo interativo: Executa o script de visualizacao
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    } else {
        // Modo batch: Executa o script passado como argumento
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    // 6. Limpeza de memoria
    delete visManager;
    delete runManager;

    return 0;
}
