#include "FP_Greedy.hh"
#include "Random.hh"

// Pair of (weight, job_index)
typedef pair<size_t, size_t> pi;

/*!
 * @brief Perform random tie break in a vector: if there are elements with equal
 * weight try to swap them.
 * @param[in] pq    Vector of jobs.
 */
static void randomTieBreak(vector<pi>& pq) {
    if (pq.size() > 0) {
        size_t equal = 0;
        for (size_t i = 0; i < pq.size() - 1; ++i) {
            equal = 1;
            for (size_t j = i + 1; j < pq.size(); ++j) {
                if (pq[i].first == pq[j].first) {
                    DEBUG_MSG("Job " << pq[i].second << " and " << pq[j].second
                            << " have equal weight (" << pq[i].first << ")" << 
                            endl);
                    equal++;
                    if (Random(1, equal) == 1) {
                        DEBUG_MSG("Swapping job " << pq[i].second << " and " <<
                                pq[j].second << endl;);
                        swap(pq[i], pq[j]);
                    }
                }
            }
        }
    }
}

/*!
 * @brief Compute the cost/weight of a job in order to define an order for
 * schduling. Delta can be specified in order to adjust the makespan.
 * @param[in]  in       Input class.
 * @param[in]  j        Job identifier
 * @param[in]  delta
 * @return The cost/weight of the job j.
 */
static size_t computeJobCost(const FP_Input& in, size_t j, size_t delta = 0) {
    size_t ms = in.getReleaseDate(j) + in.getTotalDuration(j) + delta;
    int t = ms - in.getDueDates(j);

    if (in.getDueDates(j) != -1) {
        if (t > 0) {
            return ms + t * in.getWeight(j);
        } else {
            return ms;
        }
    } else {
        return 0;
    }
}

/*!
 * @brief Greedy weighted solver based on a priority queue sorted by weight.
 * Works better with smaller instances.
 * @param[in] in    Input class.
 * @param[in] out   Output class.
 */
void greedyWSolver(const FP_Input& in, FP_Output& out, bool rand) {
    out.reset();

    // Priority queue sorted by weight in descending order.
    vector<pi> pq;

    for (size_t i = 0; i < in.getJobs(); ++i) {
        pq.push_back(make_pair(abs(in.getDueDates(i)) * in.getWeight(i), i));
    }

    // Sort it in descending order of weight.
    sort(pq.begin(), pq.end(), greater<pi>());

    // If random tie break is required
    if (rand) randomTieBreak(pq);

#ifndef NDEBUG
    for (size_t i = 0; i < in.getJobs(); ++i) {
        cout << "(" << pq[i].first << "," << pq[i].second << ")" << endl;
    }
    cout << endl;
#endif

    while (!pq.empty()) {
        // extract the job with less weight
        pi top = pq.front();
        out.runJob(top.second);
        out.addJob(top.second);
        pq.erase(pq.begin());
    }
}

/*!
 * @brief Greedy randomized weighted solver based on a priority queue sorted 
 * by weight. Works better with smaller instances.
 * @param[in] in    Input class.
 * @param[in] out   Output class.
 */
void greedyRandomWSolver(const FP_Input& in, FP_Output& out) {
    greedyWSolver(in, out, true);
}

/*!
 * @brief Greedy dynamic weighted solver based on a priority queue sorted by 
 * weight. Works better with bigger instances.
 * @param[in] in    Input class.
 * @param[in] out   Output class.
 */
void greedyDynamicWSolver(const FP_Input& in, FP_Output& out, bool rand) {
    out.reset();

    // Priority queue sorted by weight in descending order.
    vector<pi> pq;
    int dues_size = 0;

    for (size_t i = 0; i < in.getJobs(); ++i) {
        size_t cost = computeJobCost(in, i);
        pq.push_back( make_pair(cost, i) );

        if (cost > 0)  { // if there's due date
            dues_size++;
        }
    }

    // Sort it in descending order: jobs without due date are at the end
    sort(pq.begin(), pq.end(), greater<pi>());

    // If random tie break is required
    if (rand) randomTieBreak(pq);

#ifndef NDEBUG
    for (size_t i = 0; i < in.getJobs(); ++i) {
        cout << "(" << pq[i].first << "," << pq[i].second << ")" << endl;
    }
    cout << endl;
#endif

    while (!pq.empty()) {
        size_t ms, delta;

        // extract the job with less weight
        pi top = pq.front();
        out.runJob(top.second);
        out.addJob(top.second);
        pq.erase(pq.begin());
        dues_size--;

        // compute the latency introduced in scheduling
        ms = in.getReleaseDate(top.second) + in.getTotalDuration(top.second);
        delta = out.getJTime(top.second) - ms;

        // update costs for remaining jobs with due date
        for (int i = 0; i < dues_size; ++i) {
            size_t j = pq[i].second;
            pq[i].first = computeJobCost(in, j, delta);
        }
        if (dues_size > 0) { // reorder wrt new costs
            sort(pq.begin(), pq.begin()+dues_size, greater<pi>());
        }

        if (rand) randomTieBreak(pq);
    }
}

/*!
 * @brief Greedy dynamic randomized weighted solver based on a priority queue
 * sorted by weight. Works better with bigger instances.
 * @param[in] in    Input class.
 * @param[in] out   Output class.
 */
void greedyDynamicRandomWSolver(const FP_Input& in, FP_Output& out) {
    greedyDynamicWSolver(in, out, true);
}