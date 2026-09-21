#include "/home/mdias/Downloads/pythia8317/include/Pythia8/Pythia.h"

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <cmath>
#include <iostream>
#include <string>

// Uso: ./pythia_higgs [signal|qcd|z|ttbar]
//   signal -> gg -> H -> b bbar               (pico em ~125 GeV)
//   qcd    -> HardQCD:hardbbbar                (background dominante)
//   z      -> q qbar -> Z -> b bbar            (background eletrofraco)
//   ttbar  -> gg/qqbar -> t tbar               (background com top)
//
// Todos os modos escrevem uma TTree "events" com o branch "mH", a massa
// invariante do par b bbar de maior pT do evento, para permitir comparar
// diretamente o pico do sinal com o espectro de cada background.

int main(int argc, char* argv[]) {

    std::string process = "signal";
    if (argc > 1) {
        process = argv[1];
    }

    const int nEvents = 100000;

    Pythia8::Pythia pythia;

    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 13000.");

    std::string outputName;

    if (process == "signal") {
        // ---- Producao de Higgs por fusao de gluons, H -> b bbar ----
        pythia.readString("HiggsSM:gg2H = on");
        pythia.readString("25:m0 = 125.0");
        pythia.readString("25:onMode = off");
        pythia.readString("25:onIfMatch = 5 -5");
        outputName = "pythia_higgs_signal.root";
    }
    else if (process == "qcd") {
        // ---- QCD bb (background dominante) ----
        pythia.readString("HardQCD:hardbbbar = on");
        pythia.readString("PhaseSpace:pTHatMin = 20."); // corte no pT duro, GeV
        outputName = "pythia_higgs_qcd.root";
    }
    else if (process == "z") {
        // ---- Z(->bb) + jatos ----
        pythia.readString("WeakZ0:gmZmode = 2"); // so Z, sem gamma*
        pythia.readString("WeakSingleBoson:ffbar2gmZ = on");
        pythia.readString("23:onMode = off");
        pythia.readString("23:onIfMatch = 5 -5"); // forca Z -> b bbar
        outputName = "pythia_higgs_z.root";
    }
    else if (process == "ttbar") {
        // ---- ttbar ----
        pythia.readString("Top:gg2ttbar = on");
        pythia.readString("Top:qqbar2ttbar = on");
        pythia.readString("24:onMode = off");
        pythia.readString("24:onIfAny = 1 2 3 4 11 13 15"); // W hadronico ou leptonico
        outputName = "pythia_higgs_ttbar.root";
    }
    else {
        std::cerr << "Processo desconhecido: '" << process << "'." << std::endl;
        std::cerr << "Use um dos seguintes: signal, qcd, z, ttbar" << std::endl;
        return 1;
    }

    // Simplificacao didatica: desligamos a hadronizacao para que os quarks
    // b e bbar apareçam diretamente como particulas finais do evento gerado
    // pelo Pythia, em vez de virarem jatos de hadrons. Isso evita ter que
    // fazer reconstrucao de jatos (ex.: com FastJet) e b-tagging, que seria
    // o procedimento correto numa analise real, mas foge do escopo do
    // exercicio. O preco e que a resolucao de massa aqui e melhor do que
    // seria com jatos reconstruidos a partir de hadrons no detector.
    pythia.readString("HadronLevel:all = off");

    if (!pythia.init()) {
        std::cerr << "Erro na inicializacao do Pythia8." << std::endl;
        return 1;
    }

    TFile outputFile(outputName.c_str(), "RECREATE");
    TTree tree("events", "Eventos");

    float mH = 0.0;
    float bPt = 0.0, bEta = 0.0, bPhi = 0.0;
    float bbarPt = 0.0, bbarEta = 0.0, bbarPhi = 0.0;
    float eventWeight = 1.0;

    tree.Branch("mH", &mH, "mH/F");
    tree.Branch("bPt", &bPt, "bPt/F");
    tree.Branch("bEta", &bEta, "bEta/F");
    tree.Branch("bPhi", &bPhi, "bPhi/F");
    tree.Branch("bbarPt", &bbarPt, "bbarPt/F");
    tree.Branch("bbarEta", &bbarEta, "bbarEta/F");
    tree.Branch("bbarPhi", &bbarPhi, "bbarPhi/F");
    tree.Branch("eventWeight", &eventWeight, "eventWeight/F");

    int nSelected = 0;

    for (int eventNumber = 0; eventNumber < nEvents; ++eventNumber) {
        if (!pythia.next()) {
            continue;
        }

        TLorentzVector bQuark;
        TLorentzVector bbarQuark;

        bool foundB = false;
        bool foundBbar = false;

        double bPtMax = -1.0;
        double bbarPtMax = -1.0;

        for (int i = 0; i < pythia.event.size(); ++i) {
            const Pythia8::Particle& particle = pythia.event[i];

            if (!particle.isFinal()) {
                continue;
            }

            const int pdgId = particle.id();

            if (pdgId != 5 && pdgId != -5) {
                continue;
            }

            TLorentzVector momentum;
            momentum.SetPxPyPzE(
                particle.px(),
                particle.py(),
                particle.pz(),
                particle.e()
            );

            // Em eventos de QCD/ttbar pode haver mais de um par b bbar;
            // ficamos com o b e o bbar de maior pT em cada evento.
            if (pdgId == 5 && momentum.Pt() > bPtMax) {
                bQuark = momentum;
                bPtMax = momentum.Pt();
                foundB = true;
            }

            if (pdgId == -5 && momentum.Pt() > bbarPtMax) {
                bbarQuark = momentum;
                bbarPtMax = momentum.Pt();
                foundBbar = true;
            }
        }

        if (!foundB || !foundBbar) {
            continue;
        }

        bPt = bQuark.Pt();
        bEta = bQuark.Eta();
        bPhi = bQuark.Phi();

        bbarPt = bbarQuark.Pt();
        bbarEta = bbarQuark.Eta();
        bbarPhi = bbarQuark.Phi();

        mH = (bQuark + bbarQuark).M();

        eventWeight = 1.0;
        tree.Fill();
        ++nSelected;
    }

    outputFile.cd();
    tree.Write();
    outputFile.Close();

    pythia.stat();

    std::cout << "Processo: " << process << std::endl;
    std::cout << "Arquivo de saida: " << outputName << std::endl;
    std::cout << "Eventos selecionados (com par b bbar): " << nSelected
               << " de " << nEvents << std::endl;

    return 0;
}
