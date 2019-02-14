// File FP_Data.hh
#ifndef FP_DATA_HH
#define FP_DATA_HH

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class FP_Input {
  friend ostream& operator<<(ostream& os, const FP_Input& bs);

 public:
  FP_Input(string file_name);
  size_t getJobs() const { return jobs; }
  size_t getMachines() const { return machines; }
  size_t getDuration(size_t j, size_t m) const { return durations[j][m]; }
  size_t getReleaseDate(size_t job) const { return release_dates[job]; }
  int getDueDates(size_t job) const { return due_dates[job]; }
  size_t getWeight(size_t job) const { return weights[job]; }

 private:
  size_t jobs, machines;
  vector<vector<size_t>> durations;
  vector<size_t> release_dates;
  vector<int> due_dates;
  vector<size_t> weights;
};

class FP_Output {
  friend ostream& operator<<(ostream& os, const FP_Output& out);
  friend istream& operator>>(istream& is, FP_Output& out);

 public:
  FP_Output(const FP_Input& i);
  FP_Output& operator=(const FP_Output& out);
  size_t operator[](size_t j) const { return schedule[j]; }
  size_t& operator[](size_t j) { return schedule[j]; }

 protected:
  const FP_Input& in;
  vector<size_t> schedule;
};
#endif
