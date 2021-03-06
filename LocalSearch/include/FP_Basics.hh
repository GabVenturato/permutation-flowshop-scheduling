// File FP_Basics.hh
#ifndef FP_BASICS_HH
#define FP_BASICS_HH

#include "FP_Data.hh"

class FP_State {
  friend ostream& operator<<(ostream& os, const FP_State& st);
  friend bool operator==(const FP_State& st1, const FP_State& st2);

 public:
  FP_State(const FP_Input& in);
  FP_State& operator=(const FP_State& s);
  size_t operator[](unsigned i) const { return schedule[i]; }
  size_t& operator[](unsigned i) { return schedule[i]; }
  void ComputeTimes(size_t job_index = 0);
  size_t getEndTime(size_t j, size_t m) const { return end_times[j][m]; }
  size_t getScheduleIndex(size_t job) const;

 protected:
  const FP_Input& in;
  vector<size_t> schedule;
  vector<vector<size_t>> start_times;
  vector<vector<size_t>> end_times;
};

// Swap job j1 and job j2.
class SwapJobs {
  friend bool operator==(const SwapJobs& m1, const SwapJobs& m2);
  friend bool operator!=(const SwapJobs& m1, const SwapJobs& m2);
  friend bool operator<(const SwapJobs& m1, const SwapJobs& m2);
  friend ostream& operator<<(ostream& os, const SwapJobs& c);
  friend istream& operator>>(istream& is, SwapJobs& c);

 public:
  SwapJobs(size_t i = 0, size_t j = 0);
  size_t j1, j2;
};

// Move job from index p1 to index p2.
class MoveJob {
  friend bool operator==(const MoveJob& m1, const MoveJob& m2);
  friend bool operator!=(const MoveJob& m1, const MoveJob& m2);
  friend bool operator<(const MoveJob& m1, const MoveJob& m2);
  friend ostream& operator<<(ostream& os, const MoveJob& c);
  friend istream& operator>>(istream& is, MoveJob& c);

public:
  MoveJob(size_t i = 0, size_t j = 0);
  size_t p1, p2;
};
#endif
