#include <gtest/gtest.h>
#include <random>

#include "lib/distributions/BernoulliDistribution.hpp"
#include "lib/law-of-large-numbers/LawOfLargeNumbersSimulator.hpp"

TEST(LawOfLargeNumbersTest, BernoulliMeanConverges) {
  using namespace ptm;

  std::mt19937 rng(123);

  auto dist = std::make_shared<BernoulliDistribution>(0.3);
  LawOfLargeNumbersSimulator sim(dist);

  size_t max_n = 100000;
  size_t step = 5000;

  LLNPathResult result = sim.Simulate(rng, max_n, step);

  ASSERT_FALSE(result.entries.empty());

  const double theoretical_mean = dist->TheoreticalMean();

  double first_error = result.entries.front().abs_error;
  double last_error = result.entries.back().abs_error;
  double last_mean = result.entries.back().sample_mean;

  EXPECT_GT(first_error, last_error);

  EXPECT_NEAR(last_mean, theoretical_mean, 0.05);

  EXPECT_LT(last_error, 0.05);

  for (std::size_t i = 1; i < result.entries.size(); ++i) {
    EXPECT_EQ(result.entries[i].n, result.entries[i - 1].n + step);
  }
}

// Add your tests...
