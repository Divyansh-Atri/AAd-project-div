// quantum_simulator.h
#pragma once
#include <vector>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

using cdouble = std::complex<double>;

struct QuantumState {
    std::size_t num_qubits{};
    std::vector<cdouble> amp;
    QuantumState() = default;
    explicit QuantumState(std::size_t n);
    std::size_t dim() const { return amp.size(); }
    void set_zero();
    void initialize_basis(std::size_t index);
    void initialize_uniform();
    void apply_single_qubit_gate(std::size_t q, const cdouble U[2][2]);
    void apply_pauli_x(std::size_t q);
    void apply_hadamard(std::size_t q);
    void apply_cnot(std::size_t control, std::size_t target);
    void apply_diffuser();
    double probability_of_index(std::size_t idx) const;
    double probability_of_any(const std::vector<std::size_t>& indices) const;
};

struct Oracle {
    virtual ~Oracle() = default;
    virtual void apply(QuantumState& psi) const = 0;
};

struct SingleIndexPhaseOracle : public Oracle {
    std::size_t target;
    explicit SingleIndexPhaseOracle(std::size_t t) : target(t) {}
    void apply(QuantumState& psi) const override;
};

struct MultiIndexPhaseOracle : public Oracle {
    std::vector<std::size_t> targets;
    explicit MultiIndexPhaseOracle(std::vector<std::size_t> t) : targets(std::move(t)) {}
    void apply(QuantumState& psi) const override;
};
