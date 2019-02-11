// File FP_Data.hh
#ifndef FP_DATA_HH
#define FP_DATA_HH

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class FP_Input 
{
  friend ostream& operator<<(ostream& os, const FP_Input& bs);
public:
  FP_Input(string file_name);
  size_t getJobs() const { return jobs; }
  size_t getMachines() const { return machines; }
  size_t getDuration(size_t j, size_t m) const { return durations[j][m]; }
  size_t getReleaseDate(size_t job) const { return release_dates[job]; }
  int getDueDates(size_t job) const { return due_dates[job]; }
  size_t getWeight(size_t job) const { return weights[job]; }
  size_t getTotalDuration(size_t job) const;
private:
  size_t jobs, machines;
  vector<vector<size_t>> durations;
  vector<size_t> release_dates;
  vector<int> due_dates;
  vector<size_t> weights;
};

class FP_Output 
{
  friend ostream& operator<<(ostream& os, const FP_Output& out);
  friend istream& operator>>(istream& is, FP_Output& out);
public:
  FP_Output(const FP_Input& i);
  FP_Output& operator=(const FP_Output& out);
  void reset();
  void addJob(size_t j) { schedule.push_back(j); }
  void runJob(size_t j);
  void addMTime(size_t t, size_t m) { machine_end_times[m] = t; }
  void addJTime(size_t t, size_t j) { job_end_times[j] = t; }
  size_t operator[](size_t j) const { return schedule[j]; }
  vector<size_t>& getSchedule() { return schedule; }
  size_t getMTime(size_t m) const { return machine_end_times[m]; }
  size_t getJTime(size_t j) const { return job_end_times[j]; }
  size_t computeMakespan() const;
  size_t computeTardiness() const;
protected:
  const FP_Input& in;
  const FP_Input& in;
  vector<size_t> schedule;
  vector<size_t> job_end_times;
  vector<size_t> machine_end_times;
};
#endif