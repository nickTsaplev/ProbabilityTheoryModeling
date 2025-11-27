#ifndef PTM_LLNPATHENTRY_HPP_
#define PTM_LLNPATHENTRY_HPP_

namespace ptm {

// Результат одной траектории для закона больших чисел
struct LLNPathEntry {
  size_t n;           // число сэмплов
  double sample_mean; // выборочное среднее
  double abs_error;   // |sample_mean - theoretical_mean|
};

} // namespace ptm

#endif // PTM_LLNPATHENTRY_HPP_
