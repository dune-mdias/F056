#include "/home/mdias/Downloads/pythia8317/include/Pythia8/Pythia.h"

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <cmath>
#include <iostream>

int main() {
    const int nEvents = 100000;

    Pythia8::Pythia pythia;

    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 13000.");

    // Ativa a produção inclusiva de pares de múons.
    pythia.readString("WeakSingleBoson:ffbar2gmZ = on");

    // Permite o decaimento do bóson virtual ou Z em múons.
    pythia.readString("23:onMode = off");
    pythia.readString("23:onIfAny = 13");

    // Apenas eventos com massa do sistema dileptônico acima de 60 GeV.
    pythia.readString("PhaseSpace:mHatMin = 60.");

    if (!pythia.init()) {
        std::cerr << "Erro na inicializacao do Pythia8." << std::endl;
        return 1;
    }

    TFile outputFile("pythia_drell_yan.root", "RECREATE");
    TTree tree("events", "Eventos pp -> mu+ mu- + X");

    int eventNumber = 0;
    int nFinal = 0;

    float muPlusPt = 0.0;
    float muPlusEta = 0.0;
    float muPlusPhi = 0.0;

    float muMinusPt = 0.0;
    float muMinusEta = 0.0;
    float muMinusPhi = 0.0;

    float invariantMass = 0.0;
    float eventWeight = 1.0;

    tree.Branch("eventNumber", &eventNumber, "eventNumber/I");
    tree.Branch("nFinal", &nFinal, "nFinal/I");

    tree.Branch("muPlusPt", &muPlusPt, "muPlusPt/F");
    tree.Branch("muPlusEta", &muPlusEta, "muPlusEta/F");
    tree.Branch("muPlusPhi", &muPlusPhi, "muPlusPhi/F");

    tree.Branch("muMinusPt", &muMinusPt, "muMinusPt/F");
    tree.Branch("muMinusEta", &muMinusEta, "muMinusEta/F");
    tree.Branch("muMinusPhi", &muMinusPhi, "muMinusPhi/F");

    tree.Branch("invariantMass", &invariantMass, "invariantMass/F");
    tree.Branch("eventWeight", &eventWeight, "eventWeight/F");

    for (eventNumber = 0; eventNumber < nEvents; ++eventNumber) {
        if (!pythia.next()) {
            continue;
        }

        TLorentzVector muPlus;
        TLorentzVector muMinus;

        bool foundMuPlus = false;
        bool foundMuMinus = false;

        nFinal = 0;

        for (int i = 0; i < pythia.event.size(); ++i) {
            const Pythia8::Particle& particle = pythia.event[i];

            if (!particle.isFinal()) {
                continue;
            }

            ++nFinal;

            const int pdgId = particle.id();

            TLorentzVector momentum;
            momentum.SetPxPyPzE(
                particle.px(),
                particle.py(),
                particle.pz(),
                particle.e()
            );

            if (pdgId == -13 && !foundMuPlus) {
                muPlus = momentum;
                foundMuPlus = true;
            }

            if (pdgId == 13 && !foundMuMinus) {
                muMinus = momentum;
                foundMuMinus = true;
            }
        }

        if (!foundMuPlus || !foundMuMinus) {
            continue;
        }

        muPlusPt = muPlus.Pt();
        muPlusEta = muPlus.Eta();
        muPlusPhi = muPlus.Phi();

        muMinusPt = muMinus.Pt();
        muMinusEta = muMinus.Eta();
        muMinusPhi = muMinus.Phi();

        invariantMass = (muPlus + muMinus).M();

        eventWeight = 1.0;
        tree.Fill();
    }

    outputFile.cd();
    tree.Write();
    outputFile.Close();

    pythia.stat();

    return 0;
}
