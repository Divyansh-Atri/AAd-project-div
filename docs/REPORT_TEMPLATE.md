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

-- The CXX compiler identification is GNU 15.2.1
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.5s)
-- Generating done (0.0s)
-- Build files have been written to: /home/divyansh/Semester 3/AAD /AAD_Project/build
[ 20%] Building CXX object CMakeFiles/grover_vs_classical.dir/src/benchmark_framework.cpp.o
[ 40%] Building CXX object CMakeFiles/grover_vs_classical.dir/src/main.cpp.o
[ 60%] Building CXX object CMakeFiles/grover_vs_classical.dir/src/grovers_algorithm.cpp.o
[ 80%] Building CXX object CMakeFiles/grover_vs_classical.dir/src/quantum_simulator.cpp.o
[100%] Linking CXX executable grover_vs_classical
[100%] Built target grover_vs_classical
Dataset Size: 4 elements
Grover's Algorithm: 3 µs, Success: 100%
Linear Search: 0 µs, Success: 100%
Binary Search: 0 µs, Success: 100%

Dataset Size: 16 elements
Grover's Algorithm: 1 µs, Success: 96.1319%
Linear Search: 0 µs, Success: 100%
Binary Search: 0 µs, Success: 100%

Dataset Size: 64 elements
Grover's Algorithm: 2 µs, Success: 99.6586%
Linear Search: 0 µs, Success: 100%
Binary Search: 0 µs, Success: 100%

Dataset Size: 256 elements
Grover's Algorithm: 10 µs, Success: 99.9947%
Linear Search: 0 µs, Success: 100%
Binary Search: 0 µs, Success: 100%

Dataset Size: 1024 elements
Grover's Algorithm: 56 µs, Success: 99.9461%
Linear Search: 0 µs, Success: 100%
Binary Search: 0 µs, Success: 100%

Dataset Size: 4096 elements
Grover's Algorithm: 422 µs, Success: 99.9945%
Linear Search: 0 µs, Success: 100%
Binary Search: 0 µs, Success: 100%


Complexity & Space Usage Table:
N       Grover(µs)    Linear(µs)    Binary(µs)    Quantum Mem (KB)    Classical Mem (KB)
4       3              0              0              0.062               0.016
16      1              0              0              0.250               0.062
64      2              0              0              1.000               0.250
256     10             0              0              4.000               1.000
1024    56             0              0              16.000              4.000
4096    422            0              0              64.000              16.000

=== Terminal Graph (Relative Execution Times) ===

N = 4 elements
Grover     | ################################################## (3.00 µs)
Linear     |  (0.00 µs)
Binary     |  (0.00 µs)

N = 16 elements
Grover     | ################################################## (1.00 µs)
Linear     |  (0.00 µs)
Binary     |  (0.00 µs)

N = 64 elements
Grover     | ################################################## (2.00 µs)
Linear     |  (0.00 µs)
Binary     |  (0.00 µs)

N = 256 elements
Grover     | ################################################## (10.00 µs)
Linear     |  (0.00 µs)
Binary     |  (0.00 µs)

N = 1024 elements
Grover     | ################################################## (56.00 µs)
Linear     |  (0.00 µs)
Binary     |  (0.00 µs)

N = 4096 elements
Grover     | ################################################## (422.00 µs)
Linear     |  (0.00 µs)
Binary     |  (0.00 µs)

=== Memory Usage & Space Complexity Analysis ===
Quantum state memory grows exponentially with qubits (2^n complex amplitudes),
while classical arrays grow linearly with dataset size (N elements).

Approximate space usage for each dataset:
 - N = 4      | Quantum: 0.06       KB | Classical: 0.02       KB
 - N = 16     | Quantum: 0.25       KB | Classical: 0.06       KB
 - N = 64     | Quantum: 1.00       KB | Classical: 0.25       KB
 - N = 256    | Quantum: 4.00       KB | Classical: 1.00       KB
 - N = 1024   | Quantum: 16.00      KB | Classical: 4.00       KB
 - N = 4096   | Quantum: 64.00      KB | Classical: 16.00      KB

🧠 Observations:
 - Quantum memory doubles with each additional qubit (O(2^n)).
 - Classical memory increases linearly with N (O(N)).
 - Even small quantum systems quickly exceed classical memory limits.
   For example, 20 qubits → ~16 MB; 30 qubits → ~16 GB.

=== Algorithmic Comparison ===
Grover's Algorithm provides a *quadratic speedup* (O(√N)) over Linear Search (O(N)) for unsorted data.

Use Grover’s Algorithm when:
 - The dataset is **unsorted** or unindexed.
 - A **quantum computer** or simulator is available.
 - The dataset is large enough that √N < N.

Prefer Classical Algorithms when:
 - Data is **sorted** (Binary Search → O(log N)).
 - Dataset is small; quantum overhead is higher.
 - Quantum memory or hardware is unavailable.

 Summary:
 - Linear Search: Time = O(N), Space = O(N)
 - Binary Search: Time = O(log N), Space = O(N)
 - Grover’s Algorithm: Time = O(√N), Space = O(2^n)

Conclusion:
Grover’s algorithm achieves a *quadratic theoretical speedup* in unstructured search,
at the cost of exponential memory due to quantum state representation.
Our C++ simulation validates this tradeoff — faster search complexity but higher space usage.