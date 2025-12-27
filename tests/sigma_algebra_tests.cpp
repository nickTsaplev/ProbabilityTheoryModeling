#include <sstream>

#include <gtest/gtest.h>
#include "lib/sigma-algebra/DiscreteRandomVariable.hpp"
#include "lib/sigma-algebra/Event.hpp"
#include "lib/sigma-algebra/OutcomeSpace.hpp"
#include "lib/sigma-algebra/ProbabilityMeasure.hpp"
#include "lib/sigma-algebra/SigmaAlgebra.hpp"

TEST(SigmaAlgebraTest, ProbabilityMeasureAndExpectation) {
  using namespace ptm;

  OutcomeSpace omega;
  auto w0 = omega.AddOutcome("1");
  auto w1 = omega.AddOutcome("2");
  auto w2 = omega.AddOutcome("3");

  ProbabilityMeasure P(omega);
  P.SetAtomicProbability(w0, 0.2);
  P.SetAtomicProbability(w1, 0.3);
  P.SetAtomicProbability(w2, 0.5);

  EXPECT_TRUE(P.IsValid(1e-9));

  // событие A = {1,3}
  std::vector<bool> mask(omega.GetSize(), false);
  mask[w0] = true;
  mask[w2] = true;
  Event A(mask);

  double pA = P.Probability(A);
  EXPECT_NEAR(pA, 0.7, 1e-9);

  // X(1)=1, X(2)=2, X(3)=3
  std::vector<double> X_values = {1.0, 2.0, 3.0};
  DiscreteRandomVariable X(omega, P, X_values);

  double EX = X.ExpectedValue();
  // E[X] = 1*0.2 + 2*0.3 + 3*0.5 = 2.3
  EXPECT_NEAR(EX, 2.3, 1e-9);
}

TEST(SigmaAlgebraTest, EventOperations) {
  using namespace ptm;

  OutcomeSpace omega;
  auto a = omega.AddOutcome("a");
  auto b = omega.AddOutcome("b");
  auto c = omega.AddOutcome("c");

  auto E1 = ptm::Event::Empty(omega.GetSize());
  auto E2 = ptm::Event::Full(omega.GetSize());

  EXPECT_FALSE(E1.Contains(a));
  EXPECT_TRUE(E2.Contains(a));
  EXPECT_TRUE(E2.Contains(b));
  EXPECT_TRUE(E2.Contains(c));

  auto E3 = Event::Complement(E2);
  EXPECT_FALSE(E3.Contains(a));
  EXPECT_FALSE(E3.Contains(b));
  EXPECT_FALSE(E3.Contains(c));
}

TEST(SigmaAlgebraTest, InvalidProbabilityMeasure) {
  using namespace ptm;

  OutcomeSpace omega;
  auto w0 = omega.AddOutcome("1");
  auto w1 = omega.AddOutcome("2");

  ProbabilityMeasure P(omega);
  P.SetAtomicProbability(w0, 0.6);
  P.SetAtomicProbability(w1, 0.6);

  EXPECT_FALSE(P.IsValid(1e-9));

  P.SetAtomicProbability(w1, 0.5);
  P.SetAtomicProbability(w0, 0.5);
  EXPECT_TRUE(P.IsValid(1e-9));
}

TEST(SigmaAlgebraTest, BasicSigmaAlgebra) {
  using namespace ptm;

  OutcomeSpace omega;
  auto w0 = omega.AddOutcome("1");
  auto w1 = omega.AddOutcome("2");

  std::vector<bool> empty_mask(omega.GetSize(), false);
  std::vector<bool> full_mask(omega.GetSize(), true);

  Event empty_event(empty_mask);
  Event full_event(full_mask);

  std::vector<Event> events = {empty_event, full_event};
  SigmaAlgebra sigma(omega, std::move(events));

  EXPECT_TRUE(sigma.IsSigmaAlgebra());
  EXPECT_EQ(sigma.GetOutcomeSpace().GetSize(), 2);
  EXPECT_EQ(sigma.GetEvents().size(), 2);
}

TEST(SigmaAlgebraTest, GeneratedSigmaAlgebra) {
  using namespace ptm;

  OutcomeSpace omega;
  auto w0 = omega.AddOutcome("1");
  auto w1 = omega.AddOutcome("2");
  auto w2 = omega.AddOutcome("3");

  std::vector<bool> generatorA(omega.GetSize(), false);
  generatorA[w0] = true;
  generatorA[w1] = true;
  Event A(generatorA);

  std::vector<Event> generators = {A};
  SigmaAlgebra sigma = SigmaAlgebra::Generate(omega, generators);

  EXPECT_TRUE(sigma.IsSigmaAlgebra());
  EXPECT_EQ(sigma.GetEvents().size(), 4);

  Event empty = Event::Empty(omega.GetSize());
  Event full = Event::Full(omega.GetSize());
  Event complement_A = Event::Complement(A);

  const auto& events = sigma.GetEvents();
  EXPECT_TRUE(std::ranges::find(events, empty) != events.end());
  EXPECT_TRUE(std::ranges::find(events, full) != events.end());
  EXPECT_TRUE(std::ranges::find(events, A) != events.end());
  EXPECT_TRUE(std::ranges::find(events, complement_A) != events.end());
}

TEST(SigmaAlgebraTest, GeneratedSigmaAlgebraWithTwo) {
  using namespace ptm;

  OutcomeSpace omega;
  auto w0 = omega.AddOutcome("1");
  auto w1 = omega.AddOutcome("2");
  auto w2 = omega.AddOutcome("3");
  auto w3 = omega.AddOutcome("4");

  // A = {1,2}, B = {3}
  std::vector<bool> generatorA(omega.GetSize(), false);
  generatorA[w0] = true;
  generatorA[w1] = true;
  Event A(generatorA);

  std::vector<bool> generatorB(omega.GetSize(), false);
  generatorB[w2] = true;
  Event B(generatorB);

  std::vector<Event> generators = {A, B};
  SigmaAlgebra sigma = SigmaAlgebra::Generate(omega, generators);

  EXPECT_TRUE(sigma.IsSigmaAlgebra());
  EXPECT_EQ(sigma.GetEvents().size(), 8);

  Event empty = Event::Empty(omega.GetSize());
  Event full = Event::Full(omega.GetSize());
  Event A_complement = Event::Complement(A);
  Event B_complement = Event::Complement(B);
  Event A_union_B = Event::Unite(A, B);
  Event A_inter_B = Event::Intersect(A, B);

  const auto& events = sigma.GetEvents();

  auto contains = [&events](const Event& e) { return std::ranges::find(events, e) != events.end(); };

  EXPECT_TRUE(contains(empty));
  EXPECT_TRUE(contains(full));
  EXPECT_TRUE(contains(A));
  EXPECT_TRUE(contains(B));
  EXPECT_TRUE(contains(A_complement));
  EXPECT_TRUE(contains(B_complement));
  EXPECT_TRUE(contains(A_union_B));
  EXPECT_TRUE(contains(A_inter_B));
}

// Add your tests...
