// File FP_Basics.cc
#include "FP_Basics.hh"
#include <algorithm>

FP_State::FP_State(const FP_Input& my_in) : in(my_in) {
  schedule.resize(my_in.getJobs());
  start_times.resize(in.getJobs(), vector<size_t>(in.getMachines()));
  end_times.resize(in.getJobs(), vector<size_t>(in.getMachines()));

  // Default state = [0,1,2,...,jobs-1]
  for (size_t i = 0; i < my_in.getJobs(); i++) {
    schedule[i] = i;
  }
}

FP_State& FP_State::operator=(const FP_State& st) {
  schedule = st.schedule;
  start_times = st.start_times;
  end_times = st.end_times;
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

/*!
 * @brief Computes start and end time for each job and each machine starting from `job_index`
 * @param[in] job_index     The index of the job from which to start computing the start and end times.
 */
void FP_State::ComputeTimes(size_t job_index) {
  size_t j;
  for (size_t m = 0; m < in.getMachines(); ++m) {
    for (size_t i = job_index; i < in.getJobs(); ++i) {
      j = schedule[i];
      if (m == 0 && i == 0) {
        start_times[j][m] = max<size_t>(0, in.getReleaseDate(schedule[0]));
      } else if (m == 0) {
        start_times[j][m] = max<size_t>(end_times[schedule[i-1]][m], in.getReleaseDate(j));
      } else if (i == 0) {
        start_times[j][m] = end_times[j][m-1];
      } else {
        start_times[j][m] = max<size_t>(end_times[schedule[i-1]][m], end_times[j][m-1]);
      }
      end_times[j][m] = start_times[j][m] + in.getDuration(j,m);
    }
  }
}

/*!
 * @brief Returns the index of the job passed as input in the schedule.
 * @param[in] job   The job number for which we want to get its index.
 * @return The index of the provided job inside the schedule.
 */
size_t FP_State::getScheduleIndex(size_t job) const {
  long d = distance(schedule.begin(), find(schedule.begin(), schedule.end(), job));
  if (d >= 0) return static_cast<size_t>(d);
  else throw logic_error("Negative vector index");
}

SwapJobs::SwapJobs(size_t i, size_t j) {
  j1 = i;
  j2 = j;
}

bool operator==(const SwapJobs& mv1, const SwapJobs& mv2) {
  if (mv1.j1 != mv2.j1 || mv1.j2 != mv2.j2) return false;
  return true;
}

bool operator!=(const SwapJobs& mv1, const SwapJobs& mv2) {
  if (mv1.j1 == mv1.j2 && mv2.j1 == mv2.j2) return false;
  return true;
}

bool operator<(const SwapJobs& mv1, const SwapJobs& mv2) {
  if (mv1.j1 < mv2.j1)
    return true;
  else {
    if (mv1.j1 == mv2.j1 && mv1.j2 < mv2.j2) return true;
  }
  return false;
}

istream& operator>>(istream& is, SwapJobs& mv) {
  char ch;
  is >> ch >> mv.j1 >> ch >> mv.j2 >> ch;
  return is;
}

ostream& operator<<(ostream& os, const SwapJobs& mv) {
  os << "(" << mv.j1 << "," << mv.j2 << ")";
  return os;
}

MoveJob::MoveJob(size_t i, size_t j) {
  p1 = i;
  p2 = j;
}

bool operator==(const MoveJob& mv1, const MoveJob& mv2) {
  if (mv1.p1 != mv2.p1 || mv1.p2 != mv2.p2) return false;
  return true;
}

bool operator!=(const MoveJob& mv1, const MoveJob& mv2) {
  if (mv1.p1 == mv1.p2 && mv2.p1 == mv2.p2) return false;
  return true;
}

bool operator<(const MoveJob& mv1, const MoveJob& mv2) {
  if (mv1.p1 < mv2.p1)
    return true;
  else {
    if (mv1.p1 == mv2.p1 && mv1.p2 < mv2.p2) return true;
  }
  return false;
}

istream& operator>>(istream& is, MoveJob& mv) {
  char ch;
  is >> ch >> mv.p1 >> ch >> mv.p2 >> ch;
  return is;
}

ostream& operator<<(ostream& os, const MoveJob& mv) {
  os << "(" << mv.p1 << "," << mv.p2 << ")";
  return os;
}
