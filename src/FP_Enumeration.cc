#include "FP_Enumeration.hh"
#include <cassert>
#include <algorithm>
#include <random>

/*!
 * @brief Generate first permutation by choosing the starting job such
 * that it's available at time 0.
 */
void EnumerationFPOpt::First() {
    // Store the jobs available at time 0
    for (size_t j = 0; j < in.getJobs(); j++) {
        if (in.getReleaseDate(j) == 0) {
            starting_jobs.push_back(j);
        }
    }

    // Assume there's at least one job available at time 0.
    assert(starting_jobs.size());

    random_device rd;
    mt19937 g(rd());
    shuffle(starting_jobs.begin(), starting_jobs.end(), g);
    // Randomly select the first job
    out.addJob(starting_jobs[0]);

    for (size_t j = 0; j < in.getJobs(); ++j) {
        if (j != starting_jobs[0]) {
            out.addJob(j);
        }
    }

#ifdef NDEBUG
    cout << "\nFirst solution: ";
    for (size_t i = 0; i < in.getJobs(); ++i) {
        cout << out.getJob(i) << " ";
    }
    cout << endl;
#endif

}

/*!
 * @brief Generate only the permutations which have in its first position
 * a job available at time 0.
 * @return false if there are no more permutations, true otherwise.
 */
bool EnumerationFPOpt::Next() {
    // If there are no more jobs available at time 0, we are done.
    if (starting_jobs.size() == 0) {
        return false;
    } else if (!next_permutation(out.getSchedule().begin()+1,
                out.getSchedule().end())) {
        // If there are no more permutations with that particular job as the
        // starting job, delete the current starting job
        starting_jobs.erase(starting_jobs.begin());
        
        out.getSchedule().clear();
        // extract a new job to be the 'first'
        out.addJob(starting_jobs[0]);

        for (size_t j = 0; j < in.getJobs(); ++j) {
            if (j != starting_jobs[0]) {
                out.addJob(j);
            }
        }

        next_permutation(out.getSchedule().begin()+1, out.getSchedule().end());
    }
    return true;
}

bool EnumerationFPOpt::Feasible() {
    // Redundant since we generate permutations that start with a job
    // available at time 0.
    return !in.getReleaseDate(out.getJob(0));
}

/*!
 * @brief Updates the end times of each machines and job accordingly
 * to the current schedule.
 */
void EnumerationFPOpt::computeEndTimes() {
    out.reset();

#ifdef NDEBUG
    cout << "\nTesting solution: ";
    for (size_t i = 0; i < in.getJobs(); ++i) {
        cout << out.getJob(i) << " ";
    }
    cout << endl;
#endif

    for (size_t i = 0; i < in.getJobs(); ++i) {
        size_t j = out.getJob(i);
        out.runJob(j);
    }
}

size_t EnumerationFPOpt::Cost() {
    return out.computeMakespan() + out.computeTardiness();
}
