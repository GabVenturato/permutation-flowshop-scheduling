#include "FP_Greedy.hh"
#include "Random.hh"
#include <ctgmath>

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
        //pq.push_back(make_pair(in.getReleaseDate(i) / in.getWeight(i), i));
        //pq.push_back(make_pair(in.getTotalDuration(i) * in.getWeight(i), i));
        pq.push_back(make_pair(abs(in.getDueDates(i)) * in.getWeight(i), i));
    }

    // Sort it in descending order of weight.
    //sort(pq.begin(), pq.end());
    sort(pq.begin(), pq.end(), greater<pi>());

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
 * by weight.
 * @param[in] in    Input class.
 * @param[in] out   Output class.
 */
void greedyRandomWSolver(const FP_Input& in, FP_Output& out) {
    out.reset();

    // Priority queue sorted by weight in descending order.
    vector<pi> pq;

    for (size_t i = 0; i < in.getJobs(); ++i) {
        //pq.push_back(make_pair(in.getReleaseDate(i) / in.getWeight(i), i));
        //pq.push_back(make_pair(in.getTotalDuration(i) * in.getWeight(i), i));
        pq.push_back(make_pair(abs(in.getDueDates(i)) * in.getWeight(i), i));
    }

    // Sort it in descending order of weight.
    //sort(pq.begin(), pq.end());
    sort(pq.begin(), pq.end(), greater<pi>());

    // If there are elements with equal weight we try to swap them
    size_t equal = 0;
    for (size_t i = 0; i < pq.size() - 1; ++i) {
        equal = 1;
        for (size_t j = i + 1; j < pq.size(); ++j) {
            if (pq[i].first == pq[j].first) {
                DEBUG_MSG("Job " << pq[i].second << " and " << pq[j].second <<
                        " have equal weight (" << pq[i].first << ")" << endl);
                equal++;
                if (Random(1, equal) == 1) {
                    DEBUG_MSG("Swapping job " << pq[i].second << " and " <<
                            pq[j].second << endl;);
                    swap(pq[i], pq[j]);
                }
            }
        }
    }

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
