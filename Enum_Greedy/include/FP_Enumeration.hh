#include "EnumerationOpt.hh"
#include "FP_Data.hh"

class EnumerationFPOpt : public EnumerationOpt<FP_Input, FP_Output, size_t> {
public:
    EnumerationFPOpt(const FP_Input& in) : EnumerationOpt(in) {}
protected:
    void First();
    bool Next();
    bool Feasible();
    void computeEndTimes();
    size_t Cost();
    vector<size_t> starting_jobs;
};

