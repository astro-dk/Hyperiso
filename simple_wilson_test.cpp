#include <iostream>
#include <memory>
#include <complex>
#include <iomanip>
#include "PhysicalModel/Wilson.h"
#include "Core/MemoryManager.h"

int main() {
    try {
        std::cout << "=== Simple b→sμμ Wilson Coefficients Test ===" << std::endl;
        
        // Initialize system without logger
        MemoryManager::GetInstance("Test/testInput.slha", {0, 1})->init();
        Parameters* sm = Parameters::GetInstance();
        
        // Choose NLO strategy
        auto nloStrategy = std::make_shared<SM_NLO_Strategy>();
        
        // Create WilsonManager
        double mu_match = 81.0;
        WilsonManager* wm = WilsonManager::GetInstance("NLO", mu_match, nloStrategy);
        
        // Set scale for b-quark
        double mu_b = 4.2;
        wm->setScale(mu_b);
        
        std::cout << "Matching scale: μ_match = " << mu_match << " GeV" << std::endl;
        std::cout << "Evaluation scale: μ = " << mu_b << " GeV" << std::endl;
        std::cout << std::endl;
        
        // Calculate Wilson coefficients
        std::complex<double> C7_nlo = wm->get(WilsonCoefficient::C7, 1);
        std::complex<double> C9_nlo = wm->get(WilsonCoefficient::C9, 1);
        std::complex<double> C10_nlo = wm->get(WilsonCoefficient::C10, 1);
        
        std::complex<double> C7_full = wm->get_full(WilsonCoefficient::C7, 1);
        std::complex<double> C9_full = wm->get_full(WilsonCoefficient::C9, 1);
        std::complex<double> C10_full = wm->get_full(WilsonCoefficient::C10, 1);
        
        std::cout << "NLO Contributions:" << std::endl;
        std::cout << "C7 (NLO):  " << std::fixed << std::setprecision(6) 
                  << C7_nlo.real() << " + " << C7_nlo.imag() << "i" << std::endl;
        std::cout << "C9 (NLO):  " << std::fixed << std::setprecision(6) 
                  << C9_nlo.real() << " + " << C9_nlo.imag() << "i" << std::endl;
        std::cout << "C10 (NLO): " << std::fixed << std::setprecision(6) 
                  << C10_nlo.real() << " + " << C10_nlo.imag() << "i" << std::endl;
        std::cout << std::endl;
        
        std::cout << "Full Coefficients (LO + NLO):" << std::endl;
        std::cout << "C7 (full):  " << std::fixed << std::setprecision(6) 
                  << C7_full.real() << " + " << C7_full.imag() << "i" << std::endl;
        std::cout << "C9 (full):  " << std::fixed << std::setprecision(6) 
                  << C9_full.real() << " + " << C9_full.imag() << "i" << std::endl;
        std::cout << "C10 (full): " << std::fixed << std::setprecision(6) 
                  << C10_full.real() << " + " << C10_full.imag() << "i" << std::endl;
        
        // Clean up
        WilsonManager::Cleanup();
        
        std::cout << "\n=== Test Complete ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}