#ifndef PTM_LLNPATHRESULT_HPP_
#define PTM_LLNPATHRESULT_HPP_

#include <vector>

#include "LLNPathEntry.hpp"

namespace ptm {

struct LLNPathResult {
  std::vector<ptm::LLNPathEntry> entries;
};

} // namespace ptm

#endif // PTM_LLNPATHRESULT_HPP_
