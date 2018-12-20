#include "FP_Data.hh"
#include "FP_Greedy.hh"
#include "FP_Enumeration.hh"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    string file_name, solver;

    if (argc == 1) {
        cout << "Input file name: ";
        cin >> file_name;
    } else if (argc == 2) {
        file_name = argv[1];
        solver = "wgreedy";
    } else if (argc == 3) {
        file_name = argv[1];
        solver = argv[2];
    } else {
        cerr << "Usage: " << argv[0] 
            << "<input file> [solver in {wgreedy, rwgreedy, enum}]" << endl;
        return EXIT_FAILURE;
    }

    FP_Input in(file_name);
#ifdef NDEBUG
    cout << in << endl;
#endif
    FP_Output out(in);

    bool greedy = false;
    if (solver == "wgreedy") {
        greedy = true;
        greedyWSolver(in, out);
    } else if (solver == "rwgreedy") {
        greedy = true;
        greedyRandomWSolver(in, out);
    } else if (solver == "enum") {
        EnumerationFPOpt solver(in); 
        if (solver.Search()) {
            cout << "Best solution found " << solver.BestSolution() 
                << " after   " << solver.NumSol() 
                << " iterations " << endl;
        } else{
            cout << "No solution exists" << endl;
        }
    } else {
        cerr << "Unknown solver " << solver << endl;
        return EXIT_FAILURE;
    }

    if (greedy) {
        cout << out << endl;
        cout << "Makespan: " << out.computeMakespan() << endl;
        cout << "Tardiness: " << out.computeTardiness() << endl;
        cout << "Total cost: " << out.computeTardiness() 
            + out.computeMakespan() << endl;
    }

    return EXIT_SUCCESS;
}
  
