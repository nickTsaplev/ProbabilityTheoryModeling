#include "DiscreteRandomVariable.hpp"

#include <stdexcept>

namespace ptm {
DiscreteRandomVariable::DiscreteRandomVariable(const OutcomeSpace& omega,
                                               const ProbabilityMeasure& P,
                                               std::vector<double> values) :
    omega_(omega), P_(P), values_(std::move(values)) {
  if (values_.size() != omega_.GetSize())
    throw std::invalid_argument("Number of values must match number of outcomes");

  if (!P_.IsValid(1e-10))
    throw std::invalid_argument("Probability must be valid with sum = 1");
}

std::optional<double> DiscreteRandomVariable::Value(OutcomeSpace::OutcomeId id) const {
  if (id >= values_.size())
    return std::nullopt;

  return values_[id];
}

double DiscreteRandomVariable::ExpectedValue() const {
  double result = 0.0;

  for (size_t i = 0; i < values_.size(); ++i) {
    result += values_[i] * P_.GetAtomicProbability(i);
  }

  return result;
}
} // namespace ptm
