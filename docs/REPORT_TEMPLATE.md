# Grover's Algorithm vs Classical Search — Comparative Analysis

**Student:** Divyansh Atri  
**Course:** Advanced Algorithms and Design (AAD)  
**Project:** Grover’s Quantum Search Algorithm vs Classical Search Algorithms (C++ Implementation)  

---

## 1. Objective

This project implements and compares **Grover’s Quantum Search Algorithm** with **classical search algorithms** (Linear and Binary search) using **C++**.  
The aim is to analyze their **time and space complexity**, demonstrate Grover’s **theoretical quantum advantage**, and validate performance through empirical testing.

---

## 2. Methodology

### 2.1 Quantum Simulation Approach
Since we don’t have quantum hardware, Grover’s algorithm was **simulated** in C++ using complex vector operations to represent quantum states.

Key design features:
- **Quantum state representation:** `std::vector<std::complex<double>>`
- **Quantum gates implemented:** Hadamard, Pauli-X, CNOT, Oracle, and Diffuser.
- **Grover iteration:** Oracle + Diffuser repeated optimal √N times.
- **Diffusion operation:** Implemented as reflection about mean amplitude (equivalent to `H⊗n (2|0⟩⟨0| - I) H⊗n`).

### 2.2 Classical Implementations
- **Linear Search:** O(N) sequential scan.
- **Binary Search:** O(log N) divide-and-conquer on sorted datasets.
- Implemented using C++ templates and STL containers (`std::vector`).

### 2.3 Experimental Framework
- **Language:** C++17  
- **Timing:** `std::chrono::high_resolution_clock`  
- **Datasets:** Randomly generated arrays of sizes 4, 16, and 256 elements.  
- **Performance Metrics:** Execution time (µs), success probability, and memory usage.  
- **Hardware:** Classical simulation (no real quantum hardware).

---

## 3. Results and Observations

### 3.1 Execution Output
Example console output from the experiment:

