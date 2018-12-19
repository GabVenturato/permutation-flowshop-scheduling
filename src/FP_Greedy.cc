#include "FP_Greedy.hh"
#include "Random.hh"

#ifdef NDEBUG
#define DEBUG_MSG(str) do { cout << str; } while (false)
#else
#define DEBUG_MSG(str) do { } while (false)
#endif

// Pair of (weight, job_index)
typedef pair<size_t, size_t> pi;

/*!
 * @brief Greedy weighted solver based on a priority queue sorted by weight.
 * @param[in] in    Input class.
 * @param[in] out   Output class.
 */
void greedyWSolver(const FP_Input& in, FP_Output& out) {
    out.reset();

    // Priority queue sorted by weight in descending order.
    vector<pi> pq;

    for (size_t i = 0; i < in.getJobs(); ++i) {
        pq.push_back(make_pair(in.getWeight(i) + in.getReleaseDate(i), i));
    }

    // Sort it in descending order of weight.
    sort(pq.begin(), pq.end());

#ifdef NDEBUG
    for (size_t i = 0; i < in.getJobs(); ++i) {
        cout << "(" << pq[i].first << "," << pq[i].second << ")" << endl;
    }
    cout << endl;
#endif

    while (!pq.empty()) {
        // extract the job with less weight
        pi top = pq.front();

        // If the release date is <= than the end time of the machine 0
        // then the job will start from the previous end time of the machine 0
        if (in.getReleaseDate(top.second) <= out.getMTime(0)) {
            size_t old_time = out.getMTime(0);

            DEBUG_MSG("job " << top.second << " starting on machine 0 at " 
                    << old_time);

            size_t new_time = old_time + in.getDuration(top.second, 0);
            out.addMTime(new_time, 0);
        } else {
            // otherwise it'll start from the value of the release date
            DEBUG_MSG("job " << top.second << " starting on machine 0 at " 
                    << in.getReleaseDate(top.second));

            size_t new_time = in.getReleaseDate(top.second) + 
                in.getDuration(top.second, 0);
            out.addMTime(new_time, 0);
        }

        DEBUG_MSG(" and ending on machine 0 at " << out.getMTime(0) << endl);

        for (size_t i = 1; i < in.getMachines(); ++i) {
            if (out.getMTime(i) > out.getMTime(i-1)) {
                // Start from the old time of machine i
                size_t old_time = out.getMTime(i);

                DEBUG_MSG("job " << top.second << " starting on machine " << 
                        i << " at " << old_time);

                size_t new_time = old_time + in.getDuration(top.second, i);
                out.addMTime(new_time, i);
            } else {
                // Start from the old time of the previous machine
                size_t old_time = out.getMTime(i-1);

                DEBUG_MSG("job " << top.second << " starting on machine " << 
                        i << " at " << old_time);

                size_t new_time = old_time + in.getDuration(top.second, i);
                out.addMTime(new_time, i);
            }

            DEBUG_MSG(" and ending on machine " << i << " at " 
                    << out.getMTime(i) << endl);
        }

        out.addJTime(out.getMTime(in.getMachines()-1), top.second);

        out.addJob(top.second);
        pq.erase(pq.begin());
    }
}
