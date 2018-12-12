#ifndef FP_DATA_HH
#define FP_DATA_HH
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

class FP_Input {
    friend ostream& operator<<(ostream& os, const FP_Input& in);
public:
    FP_Input(string file_name);
    size_t getJobs() const { return jobs; }
    size_t getMachines() const { return machines; }
    size_t getDuration(size_t j, size_t m) { return durations[j][m]; }
    size_t getReleaseDate(size_t job) { return release_dates[job]; }
    int getDueDates(size_t job) { return due_dates[job]; }
    size_t getWeight(size_t job) { return weights[job]; }
private:
    size_t jobs, machines;
    vector<vector<size_t>> durations;
    vector<size_t> release_dates;
    vector<int> due_dates;
    vector<size_t> weights;
};

class FP_Output {
    friend ostream& operator<<(ostream& os, const FP_Output& in);
public:
    FP_Output(const FP_Input& i);
private:
    const FP_Input& in;
};
#endif
