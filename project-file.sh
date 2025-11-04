#!/bin/bash
# Create directories
mkdir -p include src docs

# Create CMakeLists.txt
cat > CMakeLists.txt <<'EOF'
cmake_minimum_required(VERSION 3.16)
project(GroverVsClassical CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-O3 -ffast-math -funroll-loops -march=native)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SRC_FILES src/*.cpp)
add_executable(grover_vs_classical ${SRC_FILES})
EOF

# Create headers
cat > include/quantum_simulator.h <<'EOF'
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
EOF

cat > include/grovers_algorithm.h <<'EOF'
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
EOF

cat > include/classical_search.h <<'EOF'
// classical_search.h
#pragma once
#include <vector>
#include <optional>
#include <algorithm>
#include <cstddef>

template <typename T>
std::optional<std::size_t> linear_search(const std::vector<T>& a, const T& key) {
    for (std::size_t i = 0; i < a.size(); ++i)
        if (a[i] == key) return i;
    return std::nullopt;
}

template <typename T>
std::optional<std::size_t> binary_search_index(const std::vector<T>& a, const T& key) {
    std::size_t lo = 0, hi = a.size();
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (a[mid] < key) lo = mid + 1;
        else hi = mid;
    }
    if (lo < a.size() && !(a[lo] < key) && !(key < a[lo])) return lo;
    return std::nullopt;
}
EOF

cat > include/benchmark_framework.h <<'EOF'
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
EOF

# Create source stubs
cat > src/quantum_simulator.cpp <<'EOF'
// quantum_simulator.cpp
#include "quantum_simulator.h"
#include <cmath>
#include <algorithm>

static inline std::size_t pow2(std::size_t n) { return std::size_t{1} << n; }

QuantumState::QuantumState(std::size_t n) : num_qubits(n), amp(pow2(n), cdouble{0.0, 0.0}) {}
void QuantumState::set_zero() { std::fill(amp.begin(), amp.end(), cdouble{0.0, 0.0}); }
void QuantumState::initialize_basis(std::size_t index) { set_zero(); amp[index] = {1.0,0.0}; }
void QuantumState::initialize_uniform() { const double inv = 1.0/std::sqrt(double(dim())); for(auto &a: amp) a={inv,0.0}; }
void QuantumState::apply_pauli_x(std::size_t q){ static const cdouble X[2][2]={{0,1},{1,0}}; apply_single_qubit_gate(q,X); }
void QuantumState::apply_hadamard(std::size_t q){ const double s=1/std::sqrt(2.0); cdouble U[2][2]={{s,s},{s,-s}}; apply_single_qubit_gate(q,U); }
void QuantumState::apply_diffuser(){ cdouble mean{0,0}; for(auto&a:amp)mean+=a; mean/=double(dim()); for(auto&a:amp)a=2.0*mean-a; }
void SingleIndexPhaseOracle::apply(QuantumState& psi) const { psi.amp[target]=-psi.amp[target]; }
void MultiIndexPhaseOracle::apply(QuantumState& psi) const { for(auto t:targets) psi.amp[t]=-psi.amp[t]; }
EOF

cat > src/grovers_algorithm.cpp <<'EOF'
// grovers_algorithm.cpp
#include "grovers_algorithm.h"
#include <cmath>

static inline std::size_t pow2(std::size_t n){return std::size_t{1}<<n;}

Grover::Grover(GroverConfig cfg):cfg_(std::move(cfg)){}
GroverResult Grover::run(){
    std::size_t N=pow2(cfg_.num_qubits), M=std::max<std::size_t>(1,cfg_.marked.size());
    QuantumState psi(cfg_.num_qubits); psi.initialize_uniform();
    std::size_t k=std::floor((M_PI/4.0)*std::sqrt(double(N)/double(M)));
    MultiIndexPhaseOracle O(cfg_.marked);
    for(std::size_t i=0;i<k;++i){O.apply(psi); psi.apply_diffuser();}
    return {k, psi.probability_of_any(cfg_.marked)};
}
EOF

cat > src/benchmark_framework.cpp <<'EOF'
// benchmark_framework.cpp
#include "benchmark_framework.h"
#include "grovers_algorithm.h"
#include "classical_search.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>

template <typename Clock, typename F>
long long Benchmark::time_us(F&& f){
    auto t0=Clock::now(); f(); auto t1=Clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
}
std::size_t Benchmark::quantum_state_bytes(std::size_t n_qubits){return (1ULL<<n_qubits)*sizeof(std::complex<double>);}
std::size_t Benchmark::classical_array_bytes(std::size_t N){return N*sizeof(int);}
Benchmark::Benchmark(RunConfig cfg):cfg_(std::move(cfg)),rng_(cfg_.seed){}
std::vector<RunRow> Benchmark::run_all(){
    std::vector<RunRow> rows; std::uniform_int_distribution<int>d(0,1'000'000);
    for(auto N:cfg_.dataset_sizes){
        std::size_t n_qubits=std::log2(N);
        std::vector<int> data(N); for(auto&x:data)x=d(rng_);
        std::uniform_int_distribution<std::size_t>idx(0,N-1);
        std::size_t target=idx(rng_); int key=data[target];
        std::vector<int>sorted=data; std::sort(sorted.begin(),sorted.end());
        Grover g({n_qubits,{target}});
        long long gu=time_us([&]{g.run();});
        auto gr=g.run();
        long long lu=time_us([&]{linear_search(data,key);});
        long long bu=time_us([&]{binary_search_index(sorted,key);});
        rows.push_back({N,gu,gr.success_probability,lu,bu,
            quantum_state_bytes(n_qubits),classical_array_bytes(N)});
        std::cout<<\"Dataset Size: \"<<N<<\" elements\\n\"
                 <<\"Grover's Algorithm: \"<<gu<<\" µs, Success: \"<<gr.success_probability*100<<\"%\\n\"
                 <<\"Linear Search: \"<<lu<<\" µs, Success: 100%\\n\"
                 <<\"Binary Search: \"<<bu<<\" µs, Success: 100%\\n\\n\";
    }
    return rows;
}
EOF

cat > src/main.cpp <<'EOF'
// main.cpp
#include "benchmark_framework.h"
#include <iostream>

int main(){
    RunConfig cfg; cfg.dataset_sizes={4,16,256}; cfg.repeats=5;
    Benchmark bm(cfg);
    auto rows=bm.run_all();
    std::cout << "Complexity Scaling Table:\\nN\\tGrover(µs)\\tLinear(µs)\\tBinary(µs)\\n";
    for(const auto&r:rows)
        std::cout<<r.N<<'\\t'<<r.grover_us<<'\\t'<<r.lin_us<<'\\t'<<r.bin_us<<'\\n';
}
EOF

# Create report and README
cat > docs/REPORT_TEMPLATE.md <<'EOF'
# Grover's Algorithm vs Classical Search — Technical Report
Describe methodology, complexity, and performance analysis here.
EOF

cat > README.md <<'EOF'
# Build & Run
```bash
mkdir build && cd build
cmake ..
cmake --build . -j
./grover_vs_classical
