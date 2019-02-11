// File FP_Basics.cc
#include "FP_Basics.hh"

FP_State::FP_State(const FP_Input& my_in) : in(my_in) {
  schedule.resize(my_in.getJobs());
  for (size_t i = 0; i < my_in.getJobs(); i++) {
    schedule[i] = i;
  }
}

FP_State& FP_State::operator=(const FP_State& st) {
  schedule = st.schedule;
  return *this;
}

bool operator==(const FP_State& st1, const FP_State& st2) {
  for (size_t i = 0; i < st1.in.getJobs(); i++) {
    if (st1[i] != st2[i]) return false;
  }
  return true;
}

ostream& operator<<(ostream& os, const FP_State& st) {
  for (size_t i = 0; i < st.in.getJobs(); i++) {
    os << st[i];
    if (i < st.in.getJobs() - 1) os << " ";
  }
  return os;
}

SwapJobs::SwapJobs(unsigned i, unsigned j) {
  p1 = i;
  p2 = j;
}

bool operator==(const SwapJobs& mv1, const SwapJobs& mv2) {
  if (mv1.p1 != mv2.p1 || mv1.p2 != mv2.p2) return false;
  return true;
}

bool operator!=(const SwapJobs& mv1, const SwapJobs& mv2) {
  if (mv1.p1 == mv1.p2 && mv2.p1 == mv2.p2) return false;
  return true;
}

bool operator<(const SwapJobs& mv1, const SwapJobs& mv2) {
  if (mv1.p1 < mv2.p1)
    return true;
  else {
    if (mv1.p1 == mv2.p1 && mv1.p2 < mv2.p2) return true;
  }
  return false;
}

istream& operator>>(istream& is, SwapJobs& mv) {
  char ch;
  is >> ch >> mv.p1 >> ch >> mv.p2 >> ch;
  return is;
}

ostream& operator<<(ostream& os, const SwapJobs& mv) {
  os << "(" << mv.p1 << "," << mv.p2 << ")";
  return os;
}
