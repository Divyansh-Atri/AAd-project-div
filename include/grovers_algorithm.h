// grovers_algorithm.h
#pragma once
#include "quantum_simulator.h"
#include <vector>
#include <cstddef>
#include <utility>

struct GroverConfig {
    std::size_t num_qubits{};
    std::vector<std::size_t> marked;
};

struct GroverResult {
    std::size_t iterations{};
    double success_probability{};
};

class Grover {
public:
    explicit Grover(GroverConfig cfg);
    GroverResult run();
private:
    GroverConfig cfg_;
};
