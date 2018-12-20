#include "FP_Enumeration.hh"

void EnumerationFPOpt::First() {
    for (size_t i = 0; i < in.getJobs(); ++i) {
        out.addJob(i);
    }

#ifdef NDEBUG
    cout << "\nFirst solution: ";
    for (size_t i = 0; i < in.getJobs(); ++i) {
        cout << out.getJob(i) << " ";
    }
    cout << endl;
#endif

}

bool EnumerationFPOpt::Next() {
    // TODO: generare solo le permutazioni che hanno in prima posizione un
    // job disponibile al tempo 0.
    return next_permutation(out.getSchedule().begin(), out.getSchedule().end());
}

bool EnumerationFPOpt::Feasible() {
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
