// main.cpp
#include "benchmark_framework.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <thread>

using namespace std;

// Draw ASCII bars for terminal visualization
void draw_bar(const string& label, double value, double max_value) {
    int bar_width = static_cast<int>((value / max_value) * 50.0); // scale bar width
    cout << setw(10) << left << label << " | ";
    for (int i = 0; i < bar_width; ++i) cout << "#";
    cout << " (" << fixed << setprecision(2) << value << " µs)" << endl;
}

// Format memory usage in KB
double bytes_to_kb(size_t bytes) {
    return static_cast<double>(bytes) / 1024.0;
}

int main() {
    RunConfig cfg;
    cfg.dataset_sizes = {4, 16, 64, 256, 1024, 4096};
    cfg.repeats = 5;

    Benchmark bm(cfg);
    auto rows = bm.run_all();

    cout << "\nComplexity & Space Usage Table:\n";
    cout << left << setw(8) << "N"
         << setw(15) << "Grover(µs)"
         << setw(15) << "Linear(µs)"
         << setw(15) << "Binary(µs)"
         << setw(20) << "Quantum Mem (KB)"
         << setw(20) << "Classical Mem (KB)" << endl;

    double max_time = 0;
    for (const auto& r : rows)
        max_time = max({max_time, double(r.grover_us), double(r.lin_us), double(r.bin_us)});

    // Print timings + memory usage
    for (const auto& r : rows) {
        cout << left << setw(8) << r.N
             << setw(15) << r.grover_us
             << setw(15) << r.lin_us
             << setw(15) << r.bin_us
             << setw(20) << fixed << setprecision(3) << bytes_to_kb(r.quantum_bytes)
             << setw(20) << bytes_to_kb(r.classical_bytes)
             << endl;
    }

    // ===============================
    // Terminal ASCII Graph (timing)
    // ===============================
    cout << "\n=== Terminal Graph (Relative Execution Times) ===\n";
    for (const auto& r : rows) {
        cout << "\nN = " << r.N << " elements" << endl;
        double local_max = max({double(r.grover_us), double(r.lin_us), double(r.bin_us)});
        local_max = max(local_max, 1.0);
        draw_bar("Grover", r.grover_us, local_max);
        draw_bar("Linear", r.lin_us, local_max);
        draw_bar("Binary", r.bin_us, local_max);
    }

    // ===============================
    // Space Complexity Discussion
    // ===============================
    cout << "\n=== Memory Usage & Space Complexity Analysis ===\n";
    cout << "Quantum state memory grows exponentially with qubits (2^n complex amplitudes),\n"
         << "while classical arrays grow linearly with dataset size (N elements).\n\n";

    cout << "Approximate space usage for each dataset:\n";
    for (const auto& r : rows) {
        cout << " - N = " << setw(6) << r.N
             << " | Quantum: " << setw(10) << bytes_to_kb(r.quantum_bytes) << " KB"
             << " | Classical: " << setw(10) << bytes_to_kb(r.classical_bytes) << " KB"
             << endl;
    }

    cout << "\n🧠 Observations:\n";
    cout << " - Quantum memory doubles with each additional qubit (O(2^n)).\n";
    cout << " - Classical memory increases linearly with N (O(N)).\n";
    cout << " - Even small quantum systems quickly exceed classical memory limits.\n";
    cout << "   For example, 20 qubits → ~16 MB; 30 qubits → ~16 GB.\n";

    // ===============================
    // Algorithmic Discussion
    // ===============================
    cout << "\n=== Algorithmic Comparison ===\n";
    cout << "Grover's Algorithm provides a *quadratic speedup* (O(√N)) "
         << "over Linear Search (O(N)) for unsorted data.\n\n";

    cout << "Use Grover’s Algorithm when:\n";
    cout << " - The dataset is **unsorted** or unindexed.\n";
    cout << " - A **quantum computer** or simulator is available.\n";
    cout << " - The dataset is large enough that √N < N.\n\n";

    cout << "Prefer Classical Algorithms when:\n";
    cout << " - Data is **sorted** (Binary Search → O(log N)).\n";
    cout << " - Dataset is small; quantum overhead is higher.\n";
    cout << " - Quantum memory or hardware is unavailable.\n\n";

    cout << " Summary:\n";
    cout << " - Linear Search: Time = O(N), Space = O(N)\n";
    cout << " - Binary Search: Time = O(log N), Space = O(N)\n";
    cout << " - Grover’s Algorithm: Time = O(√N), Space = O(2^n)\n\n";

    cout << "Conclusion:\n";
    cout << "Grover’s algorithm achieves a *quadratic theoretical speedup* in unstructured search,\n"
         << "at the cost of exponential memory due to quantum state representation.\n"
         << "Our C++ simulation validates this tradeoff — faster search complexity but higher space usage.\n";

    cout << endl;
    return 0;
}
