#include "FP_Data.hh"
#include <fstream>

FP_Input::FP_Input(string file_name) {  
    const unsigned MAX_DIM = 100;
    char ch, buffer[MAX_DIM];

    ifstream is(file_name);
    if(!is) {
        cerr << "Cannot open input file " <<  file_name << endl;
        exit(1);
    }

    is >> buffer >> ch >> jobs >> ch;
    is >> buffer >> ch >> machines >> ch;

    durations.resize(jobs, vector<size_t>(machines));
    release_dates.resize(jobs);
    due_dates.resize(jobs);
    weights.resize(jobs);

    // read durations
    is.ignore(MAX_DIM,'['); // read "... Durations = ["
    is >> ch; // read first '|'
    for (size_t j = 0; j < jobs; j++) {	 
        for (size_t m = 0; m < machines; m++)
            is >> durations[j][m] >> ch;
    }
    is >> ch >> ch;

    // read release dates
    is.ignore(MAX_DIM,'['); // read "... ReleaseDates = ["
    for (size_t j = 0; j < jobs; j++) {
        is >> release_dates[j] >> ch;
    }

    // read due dates
    is.ignore(MAX_DIM,'['); // read "... DueDates = ["
    for (size_t j = 0; j < jobs; j++) {
        is >> due_dates[j] >> ch;
    }

    // read weights
    is.ignore(MAX_DIM,'['); // read "... Weights = ["
    for (size_t j = 0; j < jobs; j++) {
        is >> weights[j] >> ch;
    }
}

ostream& operator<<(ostream& os, const FP_Input& in) {
    os << "Machines = " << in.machines << ";" << endl;
    os << "Jobs = " << in.jobs << ";" << endl;
    os << endl;
  
    os << "Durations = [|";
    for (size_t j = 0; j < in.jobs; j++) {
        for (size_t m = 0; m < in.machines; m++) {
            os << in.durations[j][m];
            if (m < in.machines - 1) {
                os << ",";
            } else {
                os << "|" << endl;
            }
	    }
    }
    os << "];" << endl;

    os << "ReleaseDates = [";
    for (size_t j = 0; j < in.jobs; j++) {
        os << in.release_dates[j];
        if (j < in.jobs - 1) {
            os << ", ";
        } else {
            os << "];" << endl;
        }
    }

    os << "DueDates = [";
    for (size_t j = 0; j < in.jobs; j++) {
        os << in.due_dates[j];
        if (j < in.jobs - 1) {
            os << ", ";
        } else {
            os << "];" << endl;
        }
    }

    os << "Weights = [";
    for (size_t j = 0; j < in.jobs; j++) {
        os << in.weights[j];
        if (j < in.jobs - 1) {
            os << ", ";
        } else {
            os << "];" << endl;
        }
    }
  
    return os;
}

/*!
 * @brief Return the total duration of a job as the sum of the processing
 * time of each machine for that particular job.
 * @param[in] job The job for which we compute its total duration.
 * @return The completion time of the job.
 */
size_t FP_Input::getTotalDuration(size_t job) const {
    size_t res = 0;
    for (size_t m = 0; m < machines; ++m) {
        res += durations[job][m];
    }
    return res;
}

FP_Output::FP_Output(const FP_Input& my_in) 
    : in(my_in), job_end_times(in.getJobs()),
    machine_end_times(in.getMachines())
{}

FP_Output& FP_Output::operator=(const FP_Output& out) {
    schedule = out.schedule;
    machine_end_times = out.machine_end_times;
    job_end_times = out.job_end_times;
    return *this;
}

void FP_Output::reset() {
    for (size_t i = 0; i < in.getJobs(); ++i) {
        job_end_times[i] = 0;
    }

    for (size_t i = 0; i < in.getMachines(); ++i) {
        machine_end_times[i] = 0;
    }
}

/*!
 * @brief Run a specific job on all machines and update its completion
 * time and the completion time of each machine.
 * @param[in] job The job to run.
 */
void FP_Output::runJob(size_t job) {
    // If the release date is <= than the end time of the machine 0
    // then the job will start from the previous end time of the machine 0
    if (in.getReleaseDate(job) <= getMTime(0)) {
        size_t old_time = getMTime(0);

        DEBUG_MSG("Job " << job << " starting on machine 0 at " 
                << old_time);

        size_t new_time = old_time + in.getDuration(job, 0);
        addMTime(new_time, 0);
    } else {
        // otherwise it'll start from the value of the release date
        DEBUG_MSG("Job " << job << " starting on machine 0 at " 
                << in.getReleaseDate(job));

        size_t new_time = in.getReleaseDate(job) + 
            in.getDuration(job, 0);
        addMTime(new_time, 0);
    }

    DEBUG_MSG(" and ending on machine 0 at " << getMTime(0) << endl);

    for (size_t i = 1; i < in.getMachines(); ++i) {
        if (getMTime(i) > getMTime(i-1)) {
            // Start from the old time of machine i
            size_t old_time = getMTime(i);

            DEBUG_MSG("Job " << job << " starting on machine " << 
                    i << " at " << old_time);

            size_t new_time = old_time + in.getDuration(job, i);
            addMTime(new_time, i);
        } else {
            // Start from the old time of the previous machine
            size_t old_time = getMTime(i-1);

            DEBUG_MSG("Job " << job << " starting on machine " << 
                    i << " at " << old_time);

            size_t new_time = old_time + in.getDuration(job, i);
            addMTime(new_time, i);
        }

        DEBUG_MSG(" and ending on machine " << i << " at " 
                << getMTime(i) << endl);
    }

    addJTime(getMTime(in.getMachines()-1), job);
}

/*!
 * @brief Compute the Makespan.
 * @return The Makespan.
 */
size_t FP_Output::computeMakespan() const {
    // The Makespan is the end time of the last machine run on the last job
    // scheduled.
    return machine_end_times[in.getMachines()-1];
}

/*!
 * @brief Compute the tardiness of the schedule.
 * @return The tardiness.
 */
size_t FP_Output::computeTardiness() const {
    size_t tardiness = 0;
    for (size_t i = 0; i < in.getJobs(); ++i) {
        int due = in.getDueDates(i);
        // Compute the tardiness only if the job has a due date
        if (due != -1) {
            // and if its end time exceeds the due date.
            if (static_cast<size_t>(due) < job_end_times[i]) {
                tardiness += (job_end_times[i] - due) * in.getWeight(i);
            }
        }
    }

    return tardiness;
}

ostream& operator<<(ostream& os, const FP_Output& out) {
    os << "Schedule: [";
    for (size_t i = 0; i < out.in.getJobs(); ++i) {
        os << out.schedule[i];
        if (i < out.in.getJobs() - 1) {
            os << ",";
        } else {
            os << "]";
        }
    }
    os << "\n";
    return os;
}
