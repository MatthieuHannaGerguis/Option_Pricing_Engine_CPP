# Option Pricing Engine — C++ for Finance Project

## 📋 Project Information

| | |
|---|---|
| **School** | École Supérieure d'Ingénieurs Léonard-de-Vinci (ESILV) |
| **Program** | Master 1 — Financial Engineering (Ingénierie Financière) |
| **Group** | A4-IF3 |
| **Course** | C++ for Finance |
| **Professor** | J. PU |
| **Academic Year** | 2025–2026 |

## 👥 Team

- **Matthieu HANNA GERGUIS**
- **Renaud DE L'ÉPINE**
- **Ilian SEGOIN**

## 📖 Description

Full object-oriented pricing engine in C++ covering four major topics in computational finance:

- **Black-Scholes analytical pricing** for European Vanilla and Digital options (Calls & Puts), with Delta computation.
- **Cox-Ross-Rubinstein (CRR) binomial tree** for European and American options, with backward induction, closed-form formula, risk-neutral probabilities, and early exercise policy.
- **Monte Carlo simulation** for European and path-dependent (Asian) options, with incremental path generation, 95% confidence intervals, and a singleton random number generator (`MT` class using `std::mt19937`).
- **CRR as Black-Scholes approximation** — convergence of the binomial model to the continuous-time limit.

## 🏗️ Architecture

```
├── Option.h / Option.cpp                 # Abstract base class (payoff, expiry)
├── EuropeanVanillaOption                  # Call & Put (derived)
├── EuropeanDigitalOption                  # Digital Call & Put (derived)
├── AsianOption                            # Asian Call & Put (path-dependent, derived)
├── AmericanOption                         # American Call & Put (early exercise, derived)
├── BlackScholesPricer                     # Analytical BS pricing + Delta
├── CRRPricer                             # Binomial tree pricing (European & American)
├── BlackScholesMCPricer                   # Monte Carlo pricing engine
├── BinaryTree.h                           # Template binary tree data structure
├── MT.h                                   # Singleton Mersenne Twister RNG
└── Subject_Project_C++.pdf                # Project subject
```

## 🔧 Key Features

- **OOP design**: abstract classes, inheritance hierarchy, polymorphism, friend classes, pure virtual methods, enum types.
- **Arbitrage checks** in CRR constructor (D < R < U).
- **Template class** `BinaryTree<T>` for flexible tree storage (prices, booleans for exercise policy).
- **Early exercise policy** stored as `BinaryTree<bool>` for American options.
- **Monte Carlo**: no path storage, incremental estimate updates, 95% confidence intervals.
- **Convergence verification**: CRR → Black-Scholes as N increases; MC vs. analytical cross-validation.

## 🚀 Compilation

```bash
g++ -std=c++17 -O2 -o pricer *.cpp
./pricer
```

> **Note**: The `main()` function is not included in the source files as per project requirements. A separate test file is used for grading.

## 📊 Models & Methods

| Model | Options Supported | Method |
|---|---|---|
| Black-Scholes | European Vanilla, Digital | Closed-form (analytical) |
| CRR Binomial Tree | European, American | Backward induction + Closed-form |
| Monte Carlo (BS) | European, Asian | Simulated paths + Confidence intervals |

## 📚 References

- Hull, J.C. — *Options, Futures, and Other Derivatives*
- Course slides — C++ for Finance, J. PU, ESILV 2025
