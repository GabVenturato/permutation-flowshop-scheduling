#include "FP_Enumeration.hh"
#include <cassert>
#include <algorithm>

/*!
 * @brief Generate the first permutation: (0,1,...,jobs-1).
 */
void EnumerationFPOpt::First() {
    for (size_t j = 0; j < in.getJobs(); ++j) {
        out.addJob(j);
    }

#ifndef NDEBUG
    cout << "\nFirst solution: ";
    for (size_t i = 0; i < in.getJobs(); ++i) {
        cout << out.getJob(i) << " ";
    }
    cout << endl;
#endif

}

/*!
 * @brief Generate all the possible permutations.
 * @return false if there are no more permutations, true otherwise.
 */
bool EnumerationFPOpt::Next() {
    return next_permutation(out.getSchedule().begin(), out.getSchedule().end());
}

bool EnumerationFPOpt::Feasible() {
    // We have to evaluate all the permutations.
    return true;
}

/*!
 * @brief Updates the end times of each machines and job accordingly
 * to the current schedule.
 */
void EnumerationFPOpt::computeEndTimes() {
    out.reset();

#ifndef NDEBUG
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
