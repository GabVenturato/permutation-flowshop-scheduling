#include "FP_Enumeration.hh"

#ifdef NDEBUG
#define DEBUG_MSG(str) do { cout << str; } while (false)
#else
#define DEBUG_MSG(str) do { } while (false)
#endif

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
        // If the release date is <= than the end time of the machine 0
        // then the job will start from the previous end time of the machine 0
        if (in.getReleaseDate(j) <= out.getMTime(0)) {
            size_t old_time = out.getMTime(0);

            DEBUG_MSG("Job " << j << " starting on machine 0 at " 
                    << old_time);

            size_t new_time = old_time + in.getDuration(j, 0);
            out.addMTime(new_time, 0);
        } else {
            // otherwise it'll start from the value of the release date
            DEBUG_MSG("Job " << j << " starting on machine 0 at " 
                    << in.getReleaseDate(j));

            size_t new_time = in.getReleaseDate(j) + 
                in.getDuration(j, 0);
            out.addMTime(new_time, 0);
        }

        DEBUG_MSG(" and ending on machine 0 at " << out.getMTime(0) << endl);

        for (size_t m = 1; m < in.getMachines(); ++m) {
            if (out.getMTime(m) > out.getMTime(m-1)) {
                // Start from the old time of machine i
                size_t old_time = out.getMTime(m);

                DEBUG_MSG("Job " << j << " starting on machine " << 
                        m << " at " << old_time);

                size_t new_time = old_time + in.getDuration(j, m);
                out.addMTime(new_time, m);
            } else {
                // Start from the old time of the previous machine
                size_t old_time = out.getMTime(m-1);

                DEBUG_MSG("Job " << j << " starting on machine " << 
                        m << " at " << old_time);

                size_t new_time = old_time + in.getDuration(j, m);
                out.addMTime(new_time, m);
            }

            DEBUG_MSG(" and ending on machine " << m << " at " 
                    << out.getMTime(m) << endl);
        }

        out.addJTime(out.getMTime(in.getMachines()-1), j);
    }
}

size_t EnumerationFPOpt::Cost() {
    return out.computeMakespan() + out.computeTardiness();
}
