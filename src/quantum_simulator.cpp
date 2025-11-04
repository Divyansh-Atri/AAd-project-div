// quantum_simulator.cpp
#include "quantum_simulator.h"
#include <cmath>
#include <algorithm>

using namespace std;

static inline size_t pow2(size_t n) { return size_t{1} << n; }

QuantumState::QuantumState(size_t n)
    : num_qubits(n), amp(pow2(n), cdouble{0.0, 0.0}) {}

void QuantumState::set_zero() {
    fill(amp.begin(), amp.end(), cdouble{0.0, 0.0});
}

void QuantumState::initialize_basis(size_t index) {
    if (index >= dim()) throw out_of_range("basis index out of range");
    set_zero();
    amp[index] = cdouble{1.0, 0.0};
}

void QuantumState::initialize_uniform() {
    set_zero();
    const double inv = 1.0 / sqrt(double(dim()));
    for (auto& a : amp) a = cdouble{inv, 0.0};
}

void QuantumState::apply_single_qubit_gate(size_t q, const cdouble U[2][2]) {
    if (q >= num_qubits) throw out_of_range("qubit index out of range");
    const size_t N = dim();
    const size_t step = size_t{1} << q;

    for (size_t i = 0; i < N; i += (step << 1)) {
        for (size_t j = 0; j < step; ++j) {
            size_t i0 = i + j;        // |...0_q...>
            size_t i1 = i + j + step; // |...1_q...>
            cdouble a0 = amp[i0];
            cdouble a1 = amp[i1];
            amp[i0] = U[0][0] * a0 + U[0][1] * a1;
            amp[i1] = U[1][0] * a0 + U[1][1] * a1;
        }
    }
}

void QuantumState::apply_pauli_x(size_t q) {
    static const cdouble X[2][2] = {{0.0, 1.0}, {1.0, 0.0}};
    apply_single_qubit_gate(q, X);
}

void QuantumState::apply_hadamard(size_t q) {
    const double s = 1.0 / sqrt(2.0);
    cdouble U[2][2] = {{s, s}, {s, -s}};
    apply_single_qubit_gate(q, U);
}

void QuantumState::apply_cnot(size_t control, size_t target) {
    if (control >= num_qubits || target >= num_qubits || control == target)
        throw invalid_argument("invalid control/target for CNOT");
    const size_t N = dim();
    const size_t cmask = size_t{1} << control;
    const size_t tmask = size_t{1} << target;

    for (size_t i = 0; i < N; ++i) {
        if ((i & cmask) && !(i & tmask)) {
            size_t j = i | tmask; // flip target
            swap(amp[i], amp[j]);
        }
    }
}

void QuantumState::apply_diffuser() {
    cdouble mean{0.0, 0.0};
    for (const auto& a : amp) mean += a;
    mean /= double(dim());
    for (auto& a : amp) a = 2.0 * mean - a;
}

double QuantumState::probability_of_index(size_t idx) const {
    double re = real(amp[idx]);
    double im = imag(amp[idx]);
    return re * re + im * im;
}

double QuantumState::probability_of_any(const vector<size_t>& indices) const {
    double total = 0.0;
    for (auto i : indices) total += probability_of_index(i);
    return total;
}

void SingleIndexPhaseOracle::apply(QuantumState& psi) const {
    if (target >= psi.dim()) throw out_of_range("oracle target out of range");
    psi.amp[target] = -psi.amp[target];
}

void MultiIndexPhaseOracle::apply(QuantumState& psi) const {
    for (auto t : targets) {
        if (t >= psi.dim()) throw out_of_range("oracle target out of range");
        psi.amp[t] = -psi.amp[t];
    }
}
