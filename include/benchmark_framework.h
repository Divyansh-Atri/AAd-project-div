// benchmark_framework.h
#pragma once
#include <vector>
#include <cstddef>
#include <string>
#include <chrono>
#include <random>
#include <fstream>
#include <tuple>

struct RunConfig {
    std::vector<std::size_t> dataset_sizes;
    unsigned seed{42};
    std::string csv_path{"results.csv"};
    bool print_header{true};
    int repeats{5};
};

struct RunRow {
    std::size_t N{};
    long long grover_us{};
    double grover_success{};
    long long lin_us{};
    long long bin_us{};
    std::size_t quantum_bytes{};
    std::size_t classical_bytes{};
};

class Benchmark {
public:
    explicit Benchmark(RunConfig cfg);
    std::vector<RunRow> run_all();

private:
    RunConfig cfg_;
    std::mt19937_64 rng_;
    template <typename Clock = std::chrono::high_resolution_clock, typename F>
    static long long time_us(F&& f);
    static std::size_t quantum_state_bytes(std::size_t n_qubits);
    static std::size_t classical_array_bytes(std::size_t N);
};
