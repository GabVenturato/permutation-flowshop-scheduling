// File FP_Helpers.cc
#include "FP_Helpers.hh"

/***************************************************************************
 * State Manager
 ***************************************************************************/

void FP_StateManager::RandomState(FP_State& st) {
  for (unsigned i = 0; i < in.getJobs(); i++) {
    unsigned j = Random::Uniform<unsigned>(i, in.getJobs() - 1);
    swap(st[i], st[j]);
  }
}

/*****************************************************************************
 * Output Manager
 *****************************************************************************/

void FP_OutputManager::InputState(FP_State& st, const FP_Output& out) const {
  for (unsigned i = 0; i < in.getJobs(); i++) {
    st[i] = out[i];
  }
}

void FP_OutputManager::OutputState(const FP_State& st, FP_Output& out) const {
  for (unsigned i = 0; i < in.getJobs(); i++) {
    out[i] = st[i];
  }
}

/***************************************************************************
 * Cost Components
 ***************************************************************************/

int Makespan::ComputeCost(const FP_State& st) const {
  unsigned cost = 0;
  // Insert the code that computes the cost for component 1
  // of state st
  throw logic_error("FP_Makespan::ComputeCost not implemented yet");
  return cost;
}

void Makespan::PrintViolations(const FP_State& st, ostream& os) const {
  // Insert the code that prints the violations of component 1
  // of state st
  throw logic_error("FP_Makespan::PrintViolations not implemented yet");
}

int Tardiness::ComputeCost(const FP_State& st) const {
  unsigned cost = 0;
  // Insert the code that computes the cost for component 2
  // of state st
  throw logic_error("Tardiness::ComputeCost not implemented yet");
  return cost;
}

void Tardiness::PrintViolations(const FP_State& st, ostream& os) const {
  // Insert the code that prints the violations of component 1
  // of state st
  throw logic_error("Tardiness::PrintViolations not implemented yet");
}

/*****************************************************************************
 * SwapJobs Neighborhood Explorer
 *****************************************************************************/

// initial move builder
void SwapJobsNeighborhoodExplorer::RandomMove(const FP_State& st, SwapJobs& mv) const {
  mv.p1 = Random::Uniform<unsigned>(0, in.getJobs() - 1);
  unsigned a[2];
  a[0] = Random::Uniform<unsigned>(0, mv.p1 - 1);
  a[1] = Random::Uniform<unsigned>(mv.p1 + 1, in.getJobs() - 1);
  unsigned c = Random::Uniform<unsigned>(0, 1);
  mv.p2 = a[c];

  // keep always p1 < p2
  if (mv.p1 > mv.p2) swap(mv.p1, mv.p2);
}

// check move feasibility
bool SwapJobsNeighborhoodExplorer::FeasibleMove(const FP_State& st, const SwapJobs& mv) const {
  return mv.p1 < mv.p2;
}

// update the state according to the move
void SwapJobsNeighborhoodExplorer::MakeMove(FP_State& st, const SwapJobs& mv) const {
  swap(st[mv.p1], st[mv.p2]);
}

void SwapJobsNeighborhoodExplorer::FirstMove(const FP_State& st, SwapJobs& mv) const {
  mv.p1 = 0;
  mv.p2 = 1;
}

bool SwapJobsNeighborhoodExplorer::NextMove(const FP_State& st, SwapJobs& mv) const {
  if (mv.p2 < in.getJobs() - 1) {
    mv.p2++;
    return true;
  } else if (mv.p1 < in.getJobs() - 2) {
    mv.p1++;
    mv.p2 = mv.p1 + 1;
    return true;
  } else {
    return false;
  }
}

int SwapJobsDeltaMakespan::ComputeDeltaCost(const FP_State& st, const SwapJobs& mv) const {
  int cost = 0;
  // Insert the code that computes the delta cost of component 1 for move mv in state st
  throw logic_error("SwapJobsDeltaMakespan::ComputeDeltaCost not implemented yet");
  return cost;
}

int SwapJobsDeltaTardiness::ComputeDeltaCost(const FP_State& st, const SwapJobs& mv) const {
  int cost = 0;
  // Insert the code that computes the delta cost of component 1 for move mv in state st
  throw logic_error("SwapJobsDeltaTardiness::ComputeDeltaCost not implemented yet");
  return cost;
}
