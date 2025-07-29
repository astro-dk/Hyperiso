# Step-by-Step Guide: Calculating b→sμμ Wilson Coefficients in the Standard Model using HyperIso

## Table of Contents
1. [Introduction](#introduction)
2. [Prerequisites](#prerequisites)
3. [Installation](#installation)
4. [Understanding the Framework](#understanding-the-framework)
5. [Step-by-Step Calculation Process](#step-by-step-calculation-process)
6. [Advanced Usage](#advanced-usage)
7. [Troubleshooting](#troubleshooting)
8. [References](#references)

## Introduction

This guide provides a detailed walkthrough for calculating the Wilson coefficients for the rare decay b→sμμ in the Standard Model (SM) using the HyperIso library. The b→sμμ transition is described by the effective Hamiltonian:

$$\mathcal{H}_{\text{eff}} = -\frac{4G_F}{\sqrt{2}} V_{tb}V_{ts}^* \sum_{i=1}^{10} C_i(\mu) \mathcal{O}_i(\mu)$$

where $C_i(\mu)$ are the Wilson coefficients and $\mathcal{O}_i(\mu)$ are the effective operators.

## Prerequisites

### Required Software
- **Operating System**: Linux (Ubuntu 18.04+ recommended) or Windows
- **Compiler**: GCC 7.0 or higher
- **Build System**: CMake 3.10 or higher
- **Git**: For cloning the repository

### Required Knowledge
- Basic understanding of quantum field theory
- Familiarity with C++ programming
- Understanding of effective field theory concepts
- Knowledge of the Standard Model and flavor physics

## Installation

### Step 1: Clone the Repository
```bash
git clone https://github.com/Hyperiso/Hyperiso.git
cd Hyperiso
```

### Step 2: Build the Library
```bash
cmake .
make
```

### Step 3: Verify Installation
```bash
./hyperiso
```

## Understanding the Framework

### Key Components

#### 1. Wilson Coefficient Classes
The library provides three main strategies for Wilson coefficient calculations:

- **`SM_LO_Strategy`**: Leading Order (LO) calculations
- **`SM_NLO_Strategy`**: Next-to-Leading Order (NLO) calculations  
- **`SM_NNLO_Strategy`**: Next-to-Next-to-Leading Order (NNLO) calculations

#### 2. Wilson Coefficient Enumeration
The library defines Wilson coefficients through the `WilsonCoefficient` enum:

```cpp
enum class WilsonCoefficient {
    C1, C2, C3, C4, C5, C6, C7, C8, C9, C10,  // Standard operators
    CQ1, CQ2,                                   // Scalar operators
    CP1, CP2, CP3, CP4, CP5, CP6, CP7, CP8, CP9, CP10,  // Prime operators
    CPQ1, CPQ2                                  // Prime scalar operators
};
```

#### 3. WilsonManager Class
The main interface for Wilson coefficient calculations:

```cpp
class WilsonManager {
    // Get Wilson coefficient at specific order
    complex_t get(WilsonCoefficient wc, int order) const;
    
    // Get Wilson coefficient up to specified order
    complex_t get_full(WilsonCoefficient wc, int order) const;
    
    // Set the scale for calculations
    void setScale(double mu, bool set_all=false, bool traditional_basis=false);
};
```

## Step-by-Step Calculation Process

### Step 1: Initialize the System

Create a new C++ file (e.g., `wilson_calculation.cpp`) and include the necessary headers:

```cpp
#include <iostream>
#include <memory>
#include <complex>
#include "Wilson.h"
#include "MemoryManager.h"
#include "Logger.h"
```

### Step 2: Set Up Parameters

Initialize the parameter system and memory manager:

```cpp
int main() {
    // Initialize logger
    Logger* logger = Logger::getInstance();
    logger->setLevel(Logger::LogLevel::INFO);
    
    // Initialize memory manager with SLHA file
    MemoryManager::GetInstance("path/to/your/input.slha", {0, 1})->init();
    
    // Get parameters instance
    Parameters* sm = Parameters::GetInstance();
    
    return 0;
}
```

### Step 3: Choose Calculation Strategy

Select the appropriate strategy based on your precision requirements:

```cpp
// For Leading Order calculations
auto loStrategy = std::make_shared<SM_LO_Strategy>();

// For Next-to-Leading Order calculations (recommended)
auto nloStrategy = std::make_shared<SM_NLO_Strategy>();

// For Next-to-Next-to-Leading Order calculations (highest precision)
auto nnloStrategy = std::make_shared<SM_NNLO_Strategy>();
```

### Step 4: Create WilsonManager Instance

Initialize the WilsonManager with your chosen strategy:

```cpp
// Matching scale (typically 81 GeV for b→s transitions)
double mu_match = 81.0;

// Create WilsonManager instance
WilsonManager* wm = WilsonManager::GetInstance("NLO", mu_match, nloStrategy);
```

### Step 5: Set the Scale for Calculations

Set the scale at which you want to evaluate the Wilson coefficients:

```cpp
// Set scale for b-quark mass (typically ~4.2 GeV)
double mu_b = 4.2;
wm->setScale(mu_b);
```

### Step 6: Extract Wilson Coefficients

Retrieve the Wilson coefficients for b→sμμ:

```cpp
// Get C7 (magnetic dipole operator) at NLO
complex_t C7_nlo = wm->get(WilsonCoefficient::C7, 1);

// Get C9 (vector operator) at NLO  
complex_t C9_nlo = wm->get(WilsonCoefficient::C9, 1);

// Get C10 (axial-vector operator) at NLO
complex_t C10_nlo = wm->get(WilsonCoefficient::C10, 1);

// Get full Wilson coefficients (including LO + NLO)
complex_t C7_full = wm->get_full(WilsonCoefficient::C7, 1);
complex_t C9_full = wm->get_full(WilsonCoefficient::C9, 1);
complex_t C10_full = wm->get_full(WilsonCoefficient::C10, 1);
```

### Step 7: Print Results

Display the calculated Wilson coefficients:

```cpp
std::cout << "Wilson Coefficients for b→sμμ at μ = " << mu_b << " GeV:" << std::endl;
std::cout << "C7 (NLO): " << C7_nlo.real() << " + " << C7_nlo.imag() << "i" << std::endl;
std::cout << "C9 (NLO): " << C9_nlo.real() << " + " << C9_nlo.imag() << "i" << std::endl;
std::cout << "C10 (NLO): " << C10_nlo.real() << " + " << C10_nlo.imag() << "i" << std::endl;
std::cout << std::endl;
std::cout << "Full Wilson Coefficients (LO + NLO):" << std::endl;
std::cout << "C7 (full): " << C7_full.real() << " + " << C7_full.imag() << "i" << std::endl;
std::cout << "C9 (full): " << C9_full.real() << " + " << C9_full.imag() << "i" << std::endl;
std::cout << "C10 (full): " << C10_full.real() << " + " << C10_full.imag() << "i" << std::endl;
```

### Step 8: Clean Up

Clean up the WilsonManager instances:

```cpp
WilsonManager::Cleanup();
```

## Complete Example

Here's a complete working example:

```cpp
#include <iostream>
#include <memory>
#include <complex>
#include "Wilson.h"
#include "MemoryManager.h"
#include "Logger.h"

int main() {
    // Initialize logger
    Logger* logger = Logger::getInstance();
    logger->setLevel(Logger::LogLevel::INFO);
    
    // Initialize system
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
    
    // Calculate Wilson coefficients
    complex_t C7_nlo = wm->get(WilsonCoefficient::C7, 1);
    complex_t C9_nlo = wm->get(WilsonCoefficient::C9, 1);
    complex_t C10_nlo = wm->get(WilsonCoefficient::C10, 1);
    
    complex_t C7_full = wm->get_full(WilsonCoefficient::C7, 1);
    complex_t C9_full = wm->get_full(WilsonCoefficient::C9, 1);
    complex_t C10_full = wm->get_full(WilsonCoefficient::C10, 1);
    
    // Print results
    std::cout << "=== b→sμμ Wilson Coefficients ===" << std::endl;
    std::cout << "Scale: μ = " << mu_b << " GeV" << std::endl;
    std::cout << "Matching scale: μ_match = " << mu_match << " GeV" << std::endl;
    std::cout << std::endl;
    
    std::cout << "NLO Contributions:" << std::endl;
    std::cout << "C7 (NLO):  " << C7_nlo.real() << " + " << C7_nlo.imag() << "i" << std::endl;
    std::cout << "C9 (NLO):  " << C9_nlo.real() << " + " << C9_nlo.imag() << "i" << std::endl;
    std::cout << "C10 (NLO): " << C10_nlo.real() << " + " << C10_nlo.imag() << "i" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Full Coefficients (LO + NLO):" << std::endl;
    std::cout << "C7 (full):  " << C7_full.real() << " + " << C7_full.imag() << "i" << std::endl;
    std::cout << "C9 (full):  " << C9_full.real() << " + " << C9_full.imag() << "i" << std::endl;
    std::cout << "C10 (full): " << C10_full.real() << " + " << C10_full.imag() << "i" << std::endl;
    
    // Clean up
    WilsonManager::Cleanup();
    
    return 0;
}
```

## Advanced Usage

### Scale Dependence Study

To study the scale dependence of Wilson coefficients:

```cpp
void studyScaleDependence() {
    auto nloStrategy = std::make_shared<SM_NLO_Strategy>();
    WilsonManager* wm = WilsonManager::GetInstance("NLO", 81.0, nloStrategy);
    
    std::cout << "Scale Dependence of Wilson Coefficients:" << std::endl;
    std::cout << "μ (GeV)\tC7\tC9\tC10" << std::endl;
    
    for (double mu = 2.0; mu <= 10.0; mu += 0.5) {
        wm->setScale(mu);
        
        complex_t C7 = wm->get_full(WilsonCoefficient::C7, 1);
        complex_t C9 = wm->get_full(WilsonCoefficient::C9, 1);
        complex_t C10 = wm->get_full(WilsonCoefficient::C10, 1);
        
        std::cout << mu << "\t" << C7.real() << "\t" << C9.real() << "\t" << C10.real() << std::endl;
    }
}
```

### Comparison of Different Orders

To compare LO, NLO, and NNLO results:

```cpp
void compareOrders() {
    auto loStrategy = std::make_shared<SM_LO_Strategy>();
    auto nloStrategy = std::make_shared<SM_NLO_Strategy>();
    auto nnloStrategy = std::make_shared<SM_NNLO_Strategy>();
    
    double mu_b = 4.2;
    
    // LO calculation
    WilsonManager* wm_lo = WilsonManager::GetInstance("LO", 81.0, loStrategy);
    wm_lo->setScale(mu_b);
    complex_t C7_lo = wm_lo->get_full(WilsonCoefficient::C7, 0);
    
    // NLO calculation
    WilsonManager* wm_nlo = WilsonManager::GetInstance("NLO", 81.0, nloStrategy);
    wm_nlo->setScale(mu_b);
    complex_t C7_nlo = wm_nlo->get_full(WilsonCoefficient::C7, 1);
    
    // NNLO calculation
    WilsonManager* wm_nnlo = WilsonManager::GetInstance("NNLO", 81.0, nnloStrategy);
    wm_nnlo->setScale(mu_b);
    complex_t C7_nnlo = wm_nnlo->get_full(WilsonCoefficient::C7, 2);
    
    std::cout << "C7 Wilson Coefficient Comparison:" << std::endl;
    std::cout << "LO:   " << C7_lo.real() << " + " << C7_lo.imag() << "i" << std::endl;
    std::cout << "NLO:  " << C7_nlo.real() << " + " << C7_nlo.imag() << "i" << std::endl;
    std::cout << "NNLO: " << C7_nnlo.real() << " + " << C7_nnlo.imag() << "i" << std::endl;
    
    WilsonManager::Cleanup();
}
```

### Traditional vs Conventional Basis

The library supports two bases for Wilson coefficients:

```cpp
// Conventional basis (default)
wm->setScale(mu_b, false, false);

// Traditional basis
wm->setScale(mu_b, false, true);
```

## Troubleshooting

### Common Issues

#### 1. Compilation Errors
- **Issue**: Missing dependencies
- **Solution**: Ensure all prerequisites are installed and CMake configuration is correct

#### 2. Runtime Errors
- **Issue**: "mu_match should not be 0"
- **Solution**: Ensure the matching scale is properly set (typically 81 GeV)

#### 3. Incorrect Results
- **Issue**: Wilson coefficients seem wrong
- **Solution**: 
  - Check that the SLHA input file is correct
  - Verify the scale is appropriate for b→s transitions
  - Ensure the strategy matches your precision requirements

#### 4. Memory Issues
- **Issue**: Program crashes or memory leaks
- **Solution**: Always call `WilsonManager::Cleanup()` at the end of your program

### Debug Mode

Enable debug logging for detailed information:

```cpp
Logger* logger = Logger::getInstance();
logger->setLevel(Logger::LogLevel::DEBUG);
```

## References

### Theoretical Background
- Buchalla, G., Buras, A. J., & Lautenbacher, M. E. (1996). Weak decays beyond leading logarithms. Reviews of Modern Physics, 68(4), 1125.
- Misiak, M., & Steinhauser, W. (2017). Weak radiative decays of the B meson and bounds on $M_{H^\pm}$ in the Two-Higgs-Doublet Model. The European Physical Journal C, 77(3), 201.

### Library Documentation
- HyperIso GitHub Repository: https://github.com/Hyperiso/Hyperiso
- SuperIso (predecessor): https://superiso.in2p3.fr/

### Related Papers
- Bobeth, C., et al. (2000). B→Kℓℓ decays in the Standard Model. Nuclear Physics B, 576(1-2), 34-82.
- Huber, T., et al. (2006). NNLL calculation of the B→Xsγ branching ratio. Nuclear Physics B, 740(1-2), 105-138.

## Appendix: Physical Interpretation

### Wilson Coefficients in b→sμμ

The b→sμμ decay is described by the following key operators:

1. **C7**: Magnetic dipole operator $\mathcal{O}_7 = \frac{e}{16\pi^2} m_b \bar{s}_L \sigma^{\mu\nu} b_R F_{\mu\nu}$
2. **C9**: Vector operator $\mathcal{O}_9 = \frac{e^2}{16\pi^2} (\bar{s}_L \gamma_\mu b_L)(\bar{\mu} \gamma^\mu \mu)$
3. **C10**: Axial-vector operator $\mathcal{O}_{10} = \frac{e^2}{16\pi^2} (\bar{s}_L \gamma_\mu b_L)(\bar{\mu} \gamma^\mu \gamma_5 \mu)$

### Scale Dependence

The Wilson coefficients exhibit scale dependence due to QCD renormalization group evolution. The library automatically handles the running from the matching scale (typically 81 GeV) to the low scale (typically 4.2 GeV for b-quark physics).

### Precision Levels

- **LO**: Leading order in αs expansion
- **NLO**: Next-to-leading order (recommended for most applications)
- **NNLO**: Next-to-next-to-leading order (highest precision, computationally intensive)