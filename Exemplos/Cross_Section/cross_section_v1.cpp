#include "Pythia8/Pythia.h"
using namespace Pythia8;

int main() {
    Pythia pythia;
    
    // Energia do centro de massa
    pythia.readString("Beams:eCM = 13000.");
    
    // Ativa Drell-Yan (q qbar -> gamma*/Z0)
    pythia.readString("WeakSingleBoson:ffbar2gmZ = on");
    pythia.readString("PhaseSpace:mHatMin = 60.");   // mínimo 60 GeV
    pythia.readString("PhaseSpace:mHatMax = 120.");  // máximo 120 GeV (região do Z)
    // Opcional: restringir massa do par de léptons na região do Z
    // pythia.readString("PhaseSpace:mHatMin = 60.");
    // pythia.readString("PhaseSpace:mHatMax = 120.");
    
    pythia.init();
    
    // *** CRUCIAL: Loop de geração de eventos ***
    // A seção de choque só é preenchida após gerar eventos
    int nEvent = 1000;  // Número de eventos a gerar
    int nAccepted = 0;
    
    for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
        if (!pythia.next()) continue;
        ++nAccepted;
        // Aqui você poderia analisar o evento
    }
    
    // Agora a seção de choque está disponível
    double sigma_mb = pythia.info.sigmaGen();      // em mb
    double sigma_err_mb = pythia.info.sigmaErr();
    double sigma_pb = sigma_mb * 1e9;              // converter para pb
    double sigma_err_pb = sigma_err_mb * 1e9;
    
    std::cout << "=== Drell-Yan (q qbar -> Z/gamma*) a 13 TeV ===" << std::endl;
    std::cout << "Eventos gerados: " << nAccepted << " / " << nEvent << std::endl;
    std::cout << "sigma = " << sigma_pb << " ± " << sigma_err_pb << " pb" << std::endl;
    
    // Cálculo do número de eventos para LHC (140 fb^-1)
    double L_int_fb = 140.0;
    double N_events = sigma_pb * L_int_fb * 1000.0;  // L em pb^-1
    
    std::cout << "N esperado para " << L_int_fb << " fb^-1: " << N_events << std::endl;
    
    // Estatísticas completas (mostra todos os subprocessos ativos)
    pythia.stat();
    
    return 0;
}
