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

FP_Output::FP_Output(const FP_Input& my_in) : in(my_in) {
}

