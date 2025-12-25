#include "ProbabilityMeasure.hpp"

#include <stdexcept>
#include <numeric>

namespace ptm {
    ProbabilityMeasure::ProbabilityMeasure(const OutcomeSpace& omega)
        : omega_(omega)
        , atom_probs_(omega.GetSize(), 0.0) 
    {}

    void ProbabilityMeasure::SetAtomicProbability(OutcomeSpace::OutcomeId id, double p) {
        if (id >= atom_probs_.size())
            throw std::out_of_range("Id out of range");

        if (p > 1.0 || p < 0.0)
            throw std::invalid_argument("Invalid probability");

        atom_probs_[id] = p;
    }

    double ProbabilityMeasure::GetAtomicProbability(OutcomeSpace::OutcomeId id) const {
        if (id >= atom_probs_.size())
            throw std::out_of_range("Id out of range");

        return atom_probs_[id];
    }

    bool ProbabilityMeasure::IsValid(double eps) const {
        double sum = std::accumulate(atom_probs_.begin(), atom_probs_.end(), 0.0);
        return std::abs(sum - 1.0) < std::abs(eps);
    }

    double ProbabilityMeasure::Probability(const Event& event) const {
        if (event.GetSize() != omega_.GetSize())
            throw std::out_of_range("Different sizes");

        double result = 0.0;

        for (size_t i = 0; i < atom_probs_.size(); ++i) {
            if (event.Contains(i)) {
                result += atom_probs_[i];
            }
        }
    
        return result;
    }
}