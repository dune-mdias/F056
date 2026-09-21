#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr
            << "Uso: " << argv[0]
            << " arquivo.lhe arquivo.root"
            << std::endl;
        return 1;
    }

    const std::string lheFileName = argv[1];
    const std::string rootFileName = argv[2];

    std::ifstream lheFile(lheFileName);

    if (!lheFile.is_open()) {
        std::cerr << "Nao foi possivel abrir o arquivo LHE."
                  << std::endl;
        return 1;
    }

    TFile outputFile(rootFileName.c_str(), "RECREATE");
    TTree tree("events", "Eventos gerados pelo MadGraph");

    int eventNumber = 0;
    int nParticles = 0;

    float muPlusPt = 0.0;
    float muPlusEta = 0.0;
    float muPlusPhi = 0.0;

    float muMinusPt = 0.0;
    float muMinusEta = 0.0;
    float muMinusPhi = 0.0;

    float invariantMass = 0.0;
    float eventWeight = 1.0;

    tree.Branch("eventNumber", &eventNumber, "eventNumber/I");
    tree.Branch("nParticles", &nParticles, "nParticles/I");

    tree.Branch("muPlusPt", &muPlusPt, "muPlusPt/F");
    tree.Branch("muPlusEta", &muPlusEta, "muPlusEta/F");
    tree.Branch("muPlusPhi", &muPlusPhi, "muPlusPhi/F");

    tree.Branch("muMinusPt", &muMinusPt, "muMinusPt/F");
    tree.Branch("muMinusEta", &muMinusEta, "muMinusEta/F");
    tree.Branch("muMinusPhi", &muMinusPhi, "muMinusPhi/F");

    tree.Branch("invariantMass", &invariantMass, "invariantMass/F");
    tree.Branch("eventWeight", &eventWeight, "eventWeight/F");

    std::string line;

    while (std::getline(lheFile, line)) {
        if (line.find("<event>") == std::string::npos) {
            continue;
        }

        if (!std::getline(lheFile, line)) {
            break;
        }

        std::stringstream header(line);

        int processId = 0;
        double factorizationScale = 0.0;
        double alphaQED = 0.0;
        double alphaQCD = 0.0;

        header >> nParticles
               >> processId
               >> eventWeight
               >> factorizationScale
               >> alphaQED
               >> alphaQCD;

        TLorentzVector muPlus;
        TLorentzVector muMinus;

        bool foundMuPlus = false;
        bool foundMuMinus = false;

        for (int i = 0; i < nParticles; ++i) {
            if (!std::getline(lheFile, line)) {
                break;
            }

            std::stringstream particleLine(line);

            int pdgId = 0;
            int status = 0;
            int mother1 = 0;
            int mother2 = 0;
            int color1 = 0;
            int color2 = 0;

            double px = 0.0;
            double py = 0.0;
            double pz = 0.0;
            double energy = 0.0;
            double mass = 0.0;
            double lifetime = 0.0;
            double spin = 0.0;

            particleLine
                >> pdgId
                >> status
                >> mother1
                >> mother2
                >> color1
                >> color2
                >> px
                >> py
                >> pz
                >> energy
                >> mass
                >> lifetime
                >> spin;

            if (status != 1) {
                continue;
            }

            TLorentzVector momentum;
            momentum.SetPxPyPzE(px, py, pz, energy);

            if (pdgId == -13 && !foundMuPlus) {
                muPlus = momentum;
                foundMuPlus = true;
            }

            if (pdgId == 13 && !foundMuMinus) {
                muMinus = momentum;
                foundMuMinus = true;
            }
        }

        while (std::getline(lheFile, line)) {
            if (line.find("</event>") != std::string::npos) {
                break;
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

        tree.Fill();
        ++eventNumber;
    }

    outputFile.cd();
    tree.Write();
    outputFile.Close();

    lheFile.close();

    std::cout << "Arquivo ROOT criado: "
              << rootFileName
              << std::endl;

    return 0;
}
