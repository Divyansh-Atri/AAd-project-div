// benchmark_framework.cpp
#include "benchmark_framework.h"
#include "grovers_algorithm.h"
#include "classical_search.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

template <typename Clock, typename F>
long long Benchmark::time_us(F&& f) {
    auto t0 = Clock::now();
    f();
    auto t1 = Clock::now();
    return chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
}

size_t Benchmark::quantum_state_bytes(size_t n_qubits) {
    return (1ULL << n_qubits) * sizeof(complex<double>);
}

size_t Benchmark::classical_array_bytes(size_t N) {
    return N * sizeof(int);
}

Benchmark::Benchmark(RunConfig cfg) : cfg_(move(cfg)), rng_(cfg_.seed) {}

vector<RunRow> Benchmark::run_all() {
    vector<RunRow> rows;
    uniform_int_distribution<int> d(0, 1'000'000);

    for (auto N : cfg_.dataset_sizes) {
        size_t n_qubits = static_cast<size_t>(log2(N));
        vector<int> data(N);
        for (auto& x : data) x = d(rng_);

        uniform_int_distribution<size_t> idx(0, N - 1);
        size_t target = idx(rng_);
        int key = data[target];

        vector<int> sorted = data;
        sort(sorted.begin(), sorted.end());

        Grover g({n_qubits, {target}});
        long long gu = time_us([&] { g.run(); });
        auto gr = g.run();

        long long lu = time_us([&] { linear_search(data, key); });
        long long bu = time_us([&] { binary_search_index(sorted, key); });

        rows.push_back({
            N, gu, gr.success_probability, lu, bu,
            quantum_state_bytes(n_qubits), classical_array_bytes(N)
        });

        cout << "Dataset Size: " << N << " elements\n"
             << "Grover's Algorithm: " << gu << " µs, Success: "
             << gr.success_probability * 100 << "%\n"
             << "Linear Search: " << lu << " µs, Success: 100%\n"
             << "Binary Search: " << bu << " µs, Success: 100%\n\n";
    }
    return rows;
}
