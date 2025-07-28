#include <iostream>
#include <memory>
#include <complex>
#include <iomanip>
#include <fstream>
#include "Wilson.h"
#include "MemoryManager.h"
#include "Logger.h"

// Function to print Wilson coefficients in a formatted way
void printWilsonCoefficient(const std::string& name, const std::complex<double>& value) {
    std::cout << std::setw(8) << name << ": " 
              << std::fixed << std::setprecision(6)
              << std::setw(12) << value.real() 
              << " + " << std::setw(12) << value.imag() << "i" << std::endl;
}

// Function to study scale dependence
void studyScaleDependence(const std::string& strategy_name, 
                         std::shared_ptr<InitializationStrategy> strategy) {
    std::cout << "\n=== Scale Dependence Study (" << strategy_name << ") ===" << std::endl;
    
    WilsonManager* wm = WilsonManager::GetInstance(strategy_name, 81.0, strategy);
    
    std::cout << "μ (GeV)\tC7_real\tC7_imag\tC9_real\tC9_imag\tC10_real\tC10_imag" << std::endl;
    
    for (double mu = 2.0; mu <= 10.0; mu += 0.5) {
        wm->setScale(mu);
        
        int order = (strategy_name == "LO") ? 0 : (strategy_name == "NLO") ? 1 : 2;
        
        std::complex<double> C7 = wm->get_full(WilsonCoefficient::C7, order);
        std::complex<double> C9 = wm->get_full(WilsonCoefficient::C9, order);
        std::complex<double> C10 = wm->get_full(WilsonCoefficient::C10, order);
        
        std::cout << std::fixed << std::setprecision(1) << mu << "\t"
                  << std::setprecision(6) 
                  << C7.real() << "\t" << C7.imag() << "\t"
                  << C9.real() << "\t" << C9.imag() << "\t"
                  << C10.real() << "\t" << C10.imag() << std::endl;
    }
}

// Function to compare different orders
void compareOrders() {
    std::cout << "\n=== Order Comparison ===" << std::endl;
    
    auto loStrategy = std::make_shared<SM_LO_Strategy>();
    auto nloStrategy = std::make_shared<SM_NLO_Strategy>();
    auto nnloStrategy = std::make_shared<SM_NNLO_Strategy>();
    
    double mu_b = 4.2;
    
    // LO calculation
    WilsonManager* wm_lo = WilsonManager::GetInstance("LO", 81.0, loStrategy);
    wm_lo->setScale(mu_b);
    std::complex<double> C7_lo = wm_lo->get_full(WilsonCoefficient::C7, 0);
    std::complex<double> C9_lo = wm_lo->get_full(WilsonCoefficient::C9, 0);
    std::complex<double> C10_lo = wm_lo->get_full(WilsonCoefficient::C10, 0);
    
    // NLO calculation
    WilsonManager* wm_nlo = WilsonManager::GetInstance("NLO", 81.0, nloStrategy);
    wm_nlo->setScale(mu_b);
    std::complex<double> C7_nlo = wm_nlo->get_full(WilsonCoefficient::C7, 1);
    std::complex<double> C9_nlo = wm_nlo->get_full(WilsonCoefficient::C9, 1);
    std::complex<double> C10_nlo = wm_nlo->get_full(WilsonCoefficient::C10, 1);
    
    // NNLO calculation
    WilsonManager* wm_nnlo = WilsonManager::GetInstance("NNLO", 81.0, nnloStrategy);
    wm_nnlo->setScale(mu_b);
    std::complex<double> C7_nnlo = wm_nnlo->get_full(WilsonCoefficient::C7, 2);
    std::complex<double> C9_nnlo = wm_nnlo->get_full(WilsonCoefficient::C9, 2);
    std::complex<double> C10_nnlo = wm_nnlo->get_full(WilsonCoefficient::C10, 2);
    
    std::cout << "Wilson Coefficients at μ = " << mu_b << " GeV:" << std::endl;
    std::cout << std::endl;
    
    std::cout << "C7:" << std::endl;
    printWilsonCoefficient("LO", C7_lo);
    printWilsonCoefficient("NLO", C7_nlo);
    printWilsonCoefficient("NNLO", C7_nnlo);
    std::cout << std::endl;
    
    std::cout << "C9:" << std::endl;
    printWilsonCoefficient("LO", C9_lo);
    printWilsonCoefficient("NLO", C9_nlo);
    printWilsonCoefficient("NNLO", C9_nnlo);
    std::cout << std::endl;
    
    std::cout << "C10:" << std::endl;
    printWilsonCoefficient("LO", C10_lo);
    printWilsonCoefficient("NLO", C10_nlo);
    printWilsonCoefficient("NNLO", C10_nnlo);
}

// Function to save results to file
void saveResultsToFile(const std::string& filename, 
                      const std::string& strategy_name,
                      std::shared_ptr<InitializationStrategy> strategy) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    WilsonManager* wm = WilsonManager::GetInstance(strategy_name, 81.0, strategy);
    
    file << "μ (GeV),C7_real,C7_imag,C9_real,C9_imag,C10_real,C10_imag" << std::endl;
    
    for (double mu = 2.0; mu <= 10.0; mu += 0.1) {
        wm->setScale(mu);
        
        int order = (strategy_name == "LO") ? 0 : (strategy_name == "NLO") ? 1 : 2;
        
        std::complex<double> C7 = wm->get_full(WilsonCoefficient::C7, order);
        std::complex<double> C9 = wm->get_full(WilsonCoefficient::C9, order);
        std::complex<double> C10 = wm->get_full(WilsonCoefficient::C10, order);
        
        file << std::fixed << std::setprecision(1) << mu << ","
             << std::setprecision(8) 
             << C7.real() << "," << C7.imag() << ","
             << C9.real() << "," << C9.imag() << ","
             << C10.real() << "," << C10.imag() << std::endl;
    }
    
    file.close();
    std::cout << "Results saved to " << filename << std::endl;
}

int main() {
    // Initialize logger
    Logger* logger = Logger::getInstance();
    logger->setLevel(Logger::LogLevel::INFO);
    
    std::cout << "=== b→sμμ Wilson Coefficients Calculation ===" << std::endl;
    std::cout << "Using HyperIso Library" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    try {
        // Initialize system
        MemoryManager::GetInstance("Test/testInput.slha", {0, 1})->init();
        Parameters* sm = Parameters::GetInstance();
        
        // Choose NLO strategy (recommended for most applications)
        auto nloStrategy = std::make_shared<SM_NLO_Strategy>();
        
        // Create WilsonManager
        double mu_match = 81.0;
        WilsonManager* wm = WilsonManager::GetInstance("NLO", mu_match, nloStrategy);
        
        // Set scale for b-quark
        double mu_b = 4.2;
        wm->setScale(mu_b);
        
        std::cout << "\n=== Basic Calculation ===" << std::endl;
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
        printWilsonCoefficient("C7", C7_nlo);
        printWilsonCoefficient("C9", C9_nlo);
        printWilsonCoefficient("C10", C10_nlo);
        std::cout << std::endl;
        
        std::cout << "Full Coefficients (LO + NLO):" << std::endl;
        printWilsonCoefficient("C7", C7_full);
        printWilsonCoefficient("C9", C9_full);
        printWilsonCoefficient("C10", C10_full);
        
        // Study scale dependence
        studyScaleDependence("NLO", nloStrategy);
        
        // Compare different orders
        compareOrders();
        
        // Save results to file
        saveResultsToFile("wilson_coefficients_nlo.csv", "NLO", nloStrategy);
        
        // Clean up
        WilsonManager::Cleanup();
        
        std::cout << "\n=== Calculation Complete ===" << std::endl;
        std::cout << "Results have been calculated and saved to file." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}