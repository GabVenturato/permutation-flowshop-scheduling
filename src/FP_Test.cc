#include "FP_Data.hh"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    string file_name;

    if (argc == 2) {
        file_name = argv[1];
    } else if (argc == 1) {
        cout << "Input file name: ";
        cin >> file_name;
    }

    FP_Input in(file_name);
    cout << in << endl;
    FP_Output out(in);
    cout << out << endl;

    return EXIT_SUCCESS;
}
  
