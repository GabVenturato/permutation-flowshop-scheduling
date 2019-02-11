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

 protected:
  const FP_Input& in;
  vector<size_t> schedule;
};

class SwapJobs {
  friend bool operator==(const SwapJobs& m1, const SwapJobs& m2);
  friend bool operator!=(const SwapJobs& m1, const SwapJobs& m2);
  friend bool operator<(const SwapJobs& m1, const SwapJobs& m2);
  friend ostream& operator<<(ostream& os, const SwapJobs& c);
  friend istream& operator>>(istream& is, SwapJobs& c);

 public:
  SwapJobs(unsigned i = 0, unsigned j = 0);
  unsigned p1, p2;
};
#endif
