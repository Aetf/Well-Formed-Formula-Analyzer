#include <iostream>
#include <algorithm>
#include <sstream>
#include <tuple>

#include "wffanalyzer.h"

using namespace std;


int main(int argc,char* argv[])
{
    if (argc != 2)
    {
        cout<<"Invalid arguments!"<<endl<<"Usage:"<<endl<<argv[0]<<" '[WFF]'"<<endl;
        return -1;
    }

    // Get the WFF from argument.
    string exp(argv[1]);

    // analyze and calculate
    vector<string> props;
    vector<bool> result;
    if (!analyzeExpression(exp, props, result))
        cout << "Invalid expression!" << endl;

    // The WFF has been checked, and all the result has been save to result array.
    // Print table header.
    cout << "The expression is valid. Print the truth table:" << endl;
    cout << join(props, "  ") << "  " << exp << endl;
    // Print truth table
    for (uint conf = 0; conf!= result.size(); conf++) {
        for (uint i = 0; i!= props.size(); i++)
            cout << (assignmentAt(conf, i) ? 'T' : 'F') << "  ";
        cout << (result[conf] ? 'T' : 'F') << endl;
    }

    // Print DNF and CNF.
    string dnf, cnf;
    tie(dnf, cnf) = computeNF(props, result);
    cout << "The main DNF is:" << endl << dnf << endl;
    cout << "The main CNF is:" << endl << cnf << endl;
    return 0;
}
