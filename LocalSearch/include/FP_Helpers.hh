// File FP_Helpers.hh
#ifndef FP_HELPERS_HH
#define FP_HELPERS_HH

#include <easylocal.hh>
#include "FP_Basics.hh"

using namespace EasyLocal::Core;

/***************************************************************************
 * State Manager
 ***************************************************************************/

class FP_StateManager : public StateManager<FP_Input, FP_State> {
 public:
  FP_StateManager(const FP_Input& i) : StateManager<FP_Input, FP_State>(i, "FP_StateManager") {}
  void RandomState(FP_State& st);
  bool CheckConsistency(const FP_State& st) const { return true; };
};

/***************************************************************************
 * Cost Components
 ***************************************************************************/

class Makespan : public CostComponent<FP_Input, FP_State> {
 public:
  Makespan(const FP_Input& in, int w, bool hard) : CostComponent<FP_Input, FP_State>(in, w, hard, "Makespan") {}
  int ComputeCost(const FP_State& st) const;
  void PrintViolations(const FP_State& st, ostream& os = cout) const;
};

class Tardiness : public CostComponent<FP_Input, FP_State> {
 public:
  Tardiness(const FP_Input& in, int w, bool hard) : CostComponent<FP_Input, FP_State>(in, w, hard, "Tardiness") {}
  int ComputeCost(const FP_State& st) const;
  void PrintViolations(const FP_State& st, ostream& os = cout) const;
};

/***************************************************************************
 * SwapJobs Neighborhood Explorer
 ***************************************************************************/

class SwapJobsDeltaMakespan : public DeltaCostComponent<FP_Input, FP_State, SwapJobs> {
 public:
  SwapJobsDeltaMakespan(const FP_Input& in, Makespan& cc)
      : DeltaCostComponent<FP_Input, FP_State, SwapJobs>(in, cc, "SwapJobsDeltaMakespan") {}
  int ComputeDeltaCost(const FP_State& st, const SwapJobs& mv) const;
};

class SwapJobsDeltaTardiness : public DeltaCostComponent<FP_Input, FP_State, SwapJobs> {
 public:
  SwapJobsDeltaTardiness(const FP_Input& in, Tardiness& cc)
      : DeltaCostComponent<FP_Input, FP_State, SwapJobs>(in, cc, "SwapJobsDeltaTardiness") {}
  int ComputeDeltaCost(const FP_State& st, const SwapJobs& mv) const;
};

class SwapJobsNeighborhoodExplorer : public NeighborhoodExplorer<FP_Input, FP_State, SwapJobs> {
 public:
  SwapJobsNeighborhoodExplorer(const FP_Input& pin, StateManager<FP_Input, FP_State>& psm)
      : NeighborhoodExplorer<FP_Input, FP_State, SwapJobs>(pin, psm, "SwapJobsNeighborhoodExplorer") {}
  void RandomMove(const FP_State&, SwapJobs&) const;
  bool FeasibleMove(const FP_State&, const SwapJobs&) const;
  void MakeMove(FP_State&, const SwapJobs&) const;
  void FirstMove(const FP_State&, SwapJobs&) const;
  bool NextMove(const FP_State&, SwapJobs&) const;
};

/***************************************************************************
 * MoveJob Neighborhood Explorer
 ***************************************************************************/

class MoveJobDeltaMakespan : public DeltaCostComponent<FP_Input, FP_State, MoveJob> {
 public:
  MoveJobDeltaMakespan(const FP_Input& in, Makespan& cc)
      : DeltaCostComponent<FP_Input, FP_State, MoveJob>(in, cc, "MoveJobDeltaMakespan") {}
  int ComputeDeltaCost(const FP_State& st, const MoveJob& mv) const;
};

class MoveJobDeltaTardiness : public DeltaCostComponent<FP_Input, FP_State, MoveJob> {
 public:
  MoveJobDeltaTardiness(const FP_Input& in, Tardiness& cc)
      : DeltaCostComponent<FP_Input, FP_State, MoveJob>(in, cc, "MoveJobDeltaTardiness") {}
  int ComputeDeltaCost(const FP_State& st, const MoveJob& mv) const;
};

class MoveJobNeighborhoodExplorer : public NeighborhoodExplorer<FP_Input, FP_State, MoveJob> {
 public:
  MoveJobNeighborhoodExplorer(const FP_Input& pin, StateManager<FP_Input, FP_State>& psm)
      : NeighborhoodExplorer<FP_Input, FP_State, MoveJob>(pin, psm, "MoveJobNeighborhoodExplorer") {}
  void RandomMove(const FP_State&, MoveJob&) const;
  bool FeasibleMove(const FP_State&, const MoveJob&) const;
  void MakeMove(FP_State&, const MoveJob&) const;
  void FirstMove(const FP_State&, MoveJob&) const;
  bool NextMove(const FP_State&, MoveJob&) const;
};

class DeltaCostHelper {
 public:
  DeltaCostHelper(const FP_Input& in, const FP_State& st);
  size_t operator[](size_t i) const { return schedule[i]; }
  size_t& operator[](size_t i) { return schedule[i]; }
  size_t getEndTime(size_t job, size_t machine) const { return end_times[job][machine]; }
  void ComputeTimes(size_t job_index);
 protected:
  vector<vector<size_t>> end_times, start_times;
  vector<size_t> schedule;
  const FP_Input& in;
  const FP_State& st;
};

/***************************************************************************
 * Output Manager
 ***************************************************************************/
class FP_OutputManager : public OutputManager<FP_Input, FP_Output, FP_State> {
 public:
  FP_OutputManager(const FP_Input& pin) : OutputManager<FP_Input, FP_Output, FP_State>(pin, "FP_OutputManager") {}
  void InputState(FP_State&, const FP_Output&) const;
  void OutputState(const FP_State&, FP_Output&) const;
};
#endif
