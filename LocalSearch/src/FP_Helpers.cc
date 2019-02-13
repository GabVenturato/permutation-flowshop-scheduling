// File FP_Helpers.cc
#include "FP_Helpers.hh"

/***************************************************************************
 * State Manager
 ***************************************************************************/

void FP_StateManager::RandomState(FP_State& st) {
  size_t j;
  for (unsigned i = 0; i < in.getJobs(); ++i) {
    st[i] = i;
  }
  for (size_t i = 0; i < in.getJobs(); i++) {
    j = Random::Uniform<size_t>(i, in.getJobs() - 1);
    swap(st[i], st[j]);
  }
  st.ComputeTimes();
}

/*****************************************************************************
 * Output Manager
 *****************************************************************************/

void FP_OutputManager::InputState(FP_State& st, const FP_Output& out) const {
  for (unsigned i = 0; i < in.getJobs(); ++i) {
    st[i] = out[i];
  }
  st.ComputeTimes();
}

void FP_OutputManager::OutputState(const FP_State& st, FP_Output& out) const {
  for (unsigned i = 0; i < in.getJobs(); ++i) {
    out[i] = st[i];
  }
}

/***************************************************************************
 * Cost Components
 ***************************************************************************/

int Makespan::ComputeCost(const FP_State& st) const {
  return static_cast<int>(st.getEndTime(st.getSchedule( in.getJobs()-1 ), in.getMachines()-1));
}

void Makespan::PrintViolations(const FP_State& st, ostream& os) const {
  os << "Makespan is " << ComputeCost(st) << endl;
}

int Tardiness::ComputeCost(const FP_State& st) const {
  unsigned cost = 0;
  size_t j;
  for (unsigned i = 0; i < in.getJobs(); ++i) {
    j = st.getSchedule(i);
    if (in.getDueDates(j) != -1) {
      if (static_cast<size_t>(in.getDueDates(j)) < st.getEndTime(j, in.getMachines()-1)) {
        cost += (st.getEndTime(j, in.getMachines()-1) - in.getDueDates(j)) * in.getWeight(j);
      }
    }
  }
  return cost;
}

void Tardiness::PrintViolations(const FP_State& st, ostream& os) const {
  size_t j;
  for (unsigned i = 0; i < in.getJobs(); ++i) {
    j = st.getSchedule(i);
    if (in.getDueDates(j) != -1) {
      if (static_cast<size_t>(in.getDueDates(j)) < st.getEndTime(j, in.getMachines()-1)) {
        os << "Tardiness of job " << j << " is " << (st.getEndTime(j, in.getMachines()-1) - in.getDueDates(j)) *
          in.getWeight(j) << endl;
      }
    }
  }
}

/*****************************************************************************
 * SwapJobs Neighborhood Explorer
 *****************************************************************************/

// initial move builder
void SwapJobsNeighborhoodExplorer::RandomMove(const FP_State& st, SwapJobs& mv) const {
  mv.p1 = Random::Uniform<size_t>(0, in.getJobs() - 1);
  mv.p2 = Random::Uniform<size_t>(0, in.getJobs() - 2);
  if (mv.p2 >= mv.p1)
    mv.p2++;
  if (mv.p1 > mv.p2) // swap p1 and p2 so that p1 < p2
    swap(mv.p1, mv.p2);
}

// check move feasibility
bool SwapJobsNeighborhoodExplorer::FeasibleMove(const FP_State& st, const SwapJobs& mv) const {
  return mv.p1 < mv.p2;
}

// update the state according to the move
void SwapJobsNeighborhoodExplorer::MakeMove(FP_State& st, const SwapJobs& mv) const {
  size_t idp1 = st.getScheduleIndex(mv.p1);
  size_t idp2 = st.getScheduleIndex(mv.p2);
  swap(st[idp1], st[idp2]);
  st.ComputeTimes( min<size_t>(idp1, idp2) );
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
  int future_makespan = 0;
  int current_makespan = static_cast<int>(st.getEndTime(st.getSchedule( in.getJobs()-1 ), in.getMachines()-1));
  size_t idp1 = st.getScheduleIndex(mv.p1);
  size_t idp2 = st.getScheduleIndex(mv.p2);
  size_t sidx = min<size_t>(idp1,idp2);
  vector<size_t> new_schedule;
  for (size_t j = 0; j < in.getJobs(); ++j) {
    new_schedule.push_back(st.getSchedule(j));
  }
  swap(new_schedule[idp1],new_schedule[idp2]);

  // retrieve starting time from current state
  size_t start_time;
  if (sidx == 0) {
    // if swapping job 0, start time is 0 (take care of release date later)
    start_time = 0;
  } else {
    start_time = st.getEndTime(st.getSchedule(sidx-1), 0);
  }

  vector<vector<size_t>> s_times, e_times;
  s_times.resize(in.getJobs(), vector<size_t>(in.getMachines()));
  e_times.resize(in.getJobs(), vector<size_t>(in.getMachines()));

  if (sidx) {
    for (size_t m = 0; m < in.getMachines(); ++m) {
      e_times[st.getSchedule(sidx-1)][m] = st.getEndTime(st.getSchedule(sidx-1),m);
    }
  }

  size_t j;
  for (size_t m = 0; m < in.getMachines(); ++m) {
    for (size_t i = sidx; i < in.getJobs(); ++i) {
      j = new_schedule[i];
      if (m == 0 && i == 0) {
        s_times[j][m] = max<size_t>(start_time, in.getReleaseDate(new_schedule[0]));
      } else if (m == 0) {
        s_times[j][m] = max<size_t>(e_times[new_schedule[i-1]][m], in.getReleaseDate(j));
      } else if (i == 0) {
        s_times[j][m] = e_times[j][m-1];
      } else {
        s_times[j][m] = max<size_t>(e_times[new_schedule[i-1]][m], e_times[j][m-1]);
      }
      e_times[j][m] = s_times[j][m] + in.getDuration(j,m);
    }
  }

  future_makespan = static_cast<int>(e_times[new_schedule[in.getJobs()-1]][in.getMachines()-1]);

  return future_makespan - current_makespan;
}

int SwapJobsDeltaTardiness::ComputeDeltaCost(const FP_State& st, const SwapJobs& mv) const {
  int partial_future_tardiness = 0, partial_current_tardiness = 0;
  size_t idp1 = st.getScheduleIndex(mv.p1);
  size_t idp2 = st.getScheduleIndex(mv.p2);
  size_t sidx = min<size_t>(idp1,idp2);
  vector<size_t> new_schedule;
  for (size_t j = 0; j < in.getJobs(); ++j) {
    new_schedule.push_back(st.getSchedule(j));
  }
  swap(new_schedule[idp1],new_schedule[idp2]);

  size_t j;
  for (size_t i = sidx; i < in.getJobs(); ++i) {
    j = st.getSchedule(i);
    if (in.getDueDates(j) != -1) {
      if (static_cast<size_t>(in.getDueDates(j)) < st.getEndTime(j, in.getMachines()-1)) {
        partial_current_tardiness += (st.getEndTime(j, in.getMachines()-1) - in.getDueDates(j)) * in.getWeight(j);
      }
    }
  }

  // retrieve starting time from current state
  size_t start_time;
  if (sidx == 0) {
    // if swapping job 0, start time is 0 (take care of release date later)
    start_time = 0;
  } else {
    start_time = st.getEndTime(st.getSchedule(sidx-1), 0);
  }

  vector<vector<size_t>> s_times, e_times;
  s_times.resize(in.getJobs(), vector<size_t>(in.getMachines()));
  e_times.resize(in.getJobs(), vector<size_t>(in.getMachines()));

  if (sidx) {
    for (size_t m = 0; m < in.getMachines(); ++m) {
      e_times[st.getSchedule(sidx-1)][m] = st.getEndTime(st.getSchedule(sidx-1),m);
    }
  }

  for (size_t m = 0; m < in.getMachines(); ++m) {
    for (size_t i = sidx; i < in.getJobs(); ++i) {
      j = new_schedule[i];
      if (m == 0 && i == 0) {
        s_times[j][m] = max<size_t>(start_time, in.getReleaseDate(new_schedule[0]));
      } else if (m == 0) {
        s_times[j][m] = max<size_t>(e_times[new_schedule[i-1]][m], in.getReleaseDate(j));
      } else if (i == 0) {
        s_times[j][m] = e_times[j][m-1];
      } else {
        s_times[j][m] = max<size_t>(e_times[new_schedule[i-1]][m], e_times[j][m-1]);
      }
      e_times[j][m] = s_times[j][m] + in.getDuration(j,m);
    }
  }

  for (size_t i = sidx; i < in.getJobs(); ++i) {
    j = new_schedule[i];
    if (in.getDueDates(j) != -1) {
      if (static_cast<size_t>(in.getDueDates(j)) < e_times[j][in.getMachines()-1]) {
        partial_future_tardiness += (e_times[j][in.getMachines()-1] - in.getDueDates(j)) * in.getWeight(j);
      }
    }
  }

  return partial_future_tardiness - partial_current_tardiness;
}
