// File FP_Helpers.cc
#include "FP_Helpers.hh"

/***************************************************************************
 * State Manager
 ***************************************************************************/

void FP_StateManager::RandomState(FP_State& st) {
  size_t j;

  // put jobs in order from 0 to m-1
  for (unsigned i = 0; i < in.getJobs(); ++i) {
    st[i] = i;
  }

  // randomly swap couples of jobs
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

/**
 * Cost Component 1: Makespan
 * It's the end time of the last job on last machine.
 */
int Makespan::ComputeCost(const FP_State& st) const {
  return static_cast<int>(st.getEndTime(st[in.getJobs()-1], in.getMachines()-1));
}

void Makespan::PrintViolations(const FP_State& st, ostream& os) const {
  os << "Makespan is " << ComputeCost(st) << endl;
}

/**
 * Cost Component 2: Tardiness
 * Weighted difference between the end time of the job and the due date.
 */
int Tardiness::ComputeCost(const FP_State& st) const {
  unsigned cost = 0;
  size_t j;
  for (unsigned i = 0; i < in.getJobs(); ++i) {
    j = st[i];
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
    j = st[i];
    if (in.getDueDates(j) != -1) {
      if (static_cast<size_t>(in.getDueDates(j)) < st.getEndTime(j, in.getMachines()-1)) {
        os << "Tardiness of job " << j << " is " << (st.getEndTime(j, in.getMachines()-1) - in.getDueDates(j)) *
          in.getWeight(j) << endl;
      }
    }
  }
}

/*****************************************************************************
 * DeltaCostHelper
 * helper class to compute delta costs. useful to have a shared function to
 * compute times, which is the same for every delta cost component.
 *****************************************************************************/

DeltaCostHelper::DeltaCostHelper(const FP_Input& in, const FP_State& st)
  : in(in), st(st) {
  for (size_t j = 0; j < in.getJobs(); ++j) {
    schedule.push_back( st[j] );
  }
  
  start_times.resize( in.getJobs(), vector<size_t>(in.getMachines()) );
  end_times.resize( in.getJobs(), vector<size_t>(in.getMachines()) );
}

/*!
 * @brief Given a job index it computes start/end times. Note that it doesn't
 * compute the whole matrix but only the sub matrix from the job_index's row.
 * @param[in] job_index
 */
void DeltaCostHelper::ComputeTimes(size_t job_index) {
  size_t j;

  // retrieve starting time from current state
  size_t start_time;
  if (job_index == 0) {
    // if swapping job 0, start time is 0 (take care of release date later)
    start_time = 0;
  } else {
    start_time = st.getEndTime(st[job_index-1], 0);
  }

  // take end times of the previous job for all machines, if it exists
  if (job_index) {
    for (size_t m = 0; m < in.getMachines(); ++m) {
      end_times[st[job_index-1]][m] = st.getEndTime(st[job_index-1],m);
    }
  }

  // compute new start and end times
  for (size_t m = 0; m < in.getMachines(); ++m) {
    for (size_t i = job_index; i < in.getJobs(); ++i) {
      j = schedule[i];
      if (m == 0 && i == 0) {
        start_times[j][m] = max<size_t>(start_time, in.getReleaseDate(schedule[0]));
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

/*****************************************************************************
 * SwapJobs Neighborhood Explorer
 * note that to compute delta costs (for both components) it's necessary to save
 * end times, start times, the schedule, and perform the move there in these
 * auxiliary data structures.
 * Then it's necessary to recompute all times from the job swapped with
 * lower index until the last job in the schedule, for all the machines,
 * because the change will be propagated and it's not possible to compute it in
 * less than O(mn), where m is the number of jobs and n the number of machines.
 *****************************************************************************/

// initial move builder
void SwapJobsNeighborhoodExplorer::RandomMove(const FP_State& st, SwapJobs& mv) const {
  mv.j1 = Random::Uniform<size_t>(0, in.getJobs() - 1);
  mv.j2 = Random::Uniform<size_t>(0, in.getJobs() - 2);

  // to avoid null move and restore the range [0,m] of the random sample
  if (mv.j2 >= mv.j1)
    mv.j2++;

  // swap j1 and j2 so that j1 < j2
  if (mv.j1 > mv.j2)
    swap(mv.j1, mv.j2);
}

// check move feasibility
bool SwapJobsNeighborhoodExplorer::FeasibleMove(const FP_State& st, const SwapJobs& mv) const {
  return mv.j1 < mv.j2;
}

// update the state according to the move
void SwapJobsNeighborhoodExplorer::MakeMove(FP_State& st, const SwapJobs& mv) const {
  size_t idj1 = st.getScheduleIndex(mv.j1);
  size_t idj2 = st.getScheduleIndex(mv.j2);
  swap(st[idj1], st[idj2]);
  st.ComputeTimes( min<size_t>(idj1, idj2) );
}

void SwapJobsNeighborhoodExplorer::FirstMove(const FP_State& st, SwapJobs& mv) const {
  mv.j1 = 0;
  mv.j2 = 1;
}

bool SwapJobsNeighborhoodExplorer::NextMove(const FP_State& st, SwapJobs& mv) const {
  if (mv.j2 < in.getJobs() - 1) {
    mv.j2++;
    return true;
  } else if (mv.j1 < in.getJobs() - 2) {
    mv.j1++;
    mv.j2 = mv.j1 + 1;
    return true;
  } else {
    return false;
  }
}

int SwapJobsDeltaMakespan::ComputeDeltaCost(const FP_State& st, const SwapJobs& mv) const {
  int future_makespan = 0;
  int current_makespan = static_cast<int>(st.getEndTime(st[in.getJobs()-1], in.getMachines()-1));
  DeltaCostHelper h(in, st);

  // since this move is performed using job names, it's needed to retrieve their indexes
  size_t idj1 = st.getScheduleIndex(mv.j1);
  size_t idj2 = st.getScheduleIndex(mv.j2);

  // perform the move on auxiliary variable
  swap(h[idj1],h[idj2]);

  h.ComputeTimes( min<size_t>(idj1,idj2) );

  future_makespan = static_cast<int>(h.getEndTime(h[in.getJobs()-1], in.getMachines()-1));

  return future_makespan - current_makespan;
}

int SwapJobsDeltaTardiness::ComputeDeltaCost(const FP_State& st, const SwapJobs& mv) const {
  int partial_future_tardiness = 0, partial_current_tardiness = 0;
  size_t j;
  DeltaCostHelper h(in, st);

  // since this move is performed using job names, it's needed to retrieve their indexes
  size_t idj1 = st.getScheduleIndex(mv.j1);
  size_t idj2 = st.getScheduleIndex(mv.j2);

  // perform the move on auxiliary variable
  swap(h[idj1],h[idj2]);

  size_t sidx = min<size_t>(idj1,idj2);

  // compute current tardiness
  for (size_t i = sidx; i < in.getJobs(); ++i) {
    j = st[i];
    if (in.getDueDates(j) != -1) {
      if (static_cast<size_t>(in.getDueDates(j)) < st.getEndTime(j, in.getMachines()-1)) {
        partial_current_tardiness += (st.getEndTime(j, in.getMachines()-1) - in.getDueDates(j)) * in.getWeight(j);
      }
    }
  }

  h.ComputeTimes(sidx);

  // compute new tardiness
  for (size_t i = sidx; i < in.getJobs(); ++i) {
    j = h[i];
    if (in.getDueDates(j) != -1) {
      if (static_cast<size_t>(in.getDueDates(j)) < h.getEndTime(j, in.getMachines()-1)) {
        partial_future_tardiness += (h.getEndTime(j, in.getMachines()-1) - in.getDueDates(j)) * in.getWeight(j);
      }
    }
  }

  return partial_future_tardiness - partial_current_tardiness;
}

/*****************************************************************************
 * MoveJob Neighborhood Explorer
 * same note of SwapJobs move for delta costs must be done here.
 * Look for the comment above.
 *****************************************************************************/

// initial move builder
void MoveJobNeighborhoodExplorer::RandomMove(const FP_State& st, MoveJob& mv) const {
  mv.p1 = Random::Uniform<size_t>(0, in.getJobs() - 1);
  mv.p2 = Random::Uniform<size_t>(0, in.getJobs() - 2);
  if (mv.p2 >= mv.p1)
    mv.p2++;
  if (mv.p1 > mv.p2) // swap p1 and p2 so that p1 < p2
    swap(mv.p1, mv.p2);
}

// check move feasibility
bool MoveJobNeighborhoodExplorer::FeasibleMove(const FP_State& st, const MoveJob& mv) const {
  return mv.p1 < mv.p2;
}

// update the state according to the move
void MoveJobNeighborhoodExplorer::MakeMove(FP_State& st, const MoveJob& mv) const {
  size_t moving_job = st[mv.p1];
  for (size_t j = mv.p1; j < mv.p2; ++j) {
    st[j] = st[j+1];
  }
  st[mv.p2] = moving_job;
  st.ComputeTimes( mv.p1 );
}

void MoveJobNeighborhoodExplorer::FirstMove(const FP_State& st, MoveJob& mv) const {
  mv.p1 = 0;
  mv.p2 = 1;
}

bool MoveJobNeighborhoodExplorer::NextMove(const FP_State& st, MoveJob& mv) const {
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

int MoveJobDeltaMakespan::ComputeDeltaCost(const FP_State& st, const MoveJob& mv) const {
  int future_makespan = 0;
  int current_makespan = static_cast<int>(st.getEndTime(st[in.getJobs()-1], in.getMachines()-1));
  DeltaCostHelper h(in, st);
  size_t moving_job;

  // compute new schedule
  moving_job = h[mv.p1];
  for (size_t j = mv.p1; j < mv.p2; ++j) {
    h[j] = h[j+1];
  }
  h[mv.p2] = moving_job;

  h.ComputeTimes(mv.p1);

  future_makespan = static_cast<int>(h.getEndTime(h[in.getJobs()-1], in.getMachines()-1));

  return future_makespan - current_makespan;
}

int MoveJobDeltaTardiness::ComputeDeltaCost(const FP_State& st, const MoveJob& mv) const {
  int partial_future_tardiness = 0, partial_current_tardiness = 0;
  DeltaCostHelper h(in, st);
  size_t moving_job, j;

  // compute new schedule
  moving_job = h[mv.p1];
  for (size_t j = mv.p1; j < mv.p2; ++j) {
    h[j] = h[j+1];
  }
  h[mv.p2] = moving_job;

  // compute current tardiness
  for (size_t i = mv.p1; i < in.getJobs(); ++i) {
    j = st[i];
    if (in.getDueDates(j) != -1) {
      if (static_cast<size_t>(in.getDueDates(j)) < st.getEndTime(j, in.getMachines()-1)) {
        partial_current_tardiness += (st.getEndTime(j, in.getMachines()-1) - in.getDueDates(j)) * in.getWeight(j);
      }
    }
  }

  h.ComputeTimes(mv.p1);

  // compute new tardiness
  for (size_t i = mv.p1; i < in.getJobs(); ++i) {
    j = h[i];
    if (in.getDueDates(j) != -1) {
      if (static_cast<size_t>(in.getDueDates(j)) < h.getEndTime(j, in.getMachines()-1)) {
        partial_future_tardiness += (h.getEndTime(j, in.getMachines()-1) - in.getDueDates(j)) * in.getWeight(j);
      }
    }
  }

  return partial_future_tardiness - partial_current_tardiness;
}