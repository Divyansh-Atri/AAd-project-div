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
