#include <iostream>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <tuple>

#include "wffanalyzer.h"

using namespace std;


int main(int argc,char* argv[])
{
    if (argc!=2)
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
        cout<<"Invalid expression!"<<endl;

    // The WFF has been checked, and all the result has been save to result array.
    // Print table header.
    cout<<"The expression is valid. Print the truth table:"<<endl;
    for (auto i=props.begin();i!=props.end();++i)
        cout<<*i<<"  ";
    cout<<exp.substr(0,exp.size()-1)<<endl;
    // Print truth table
    bitset<MAX_PROP_VARIABLE> pi;
    pi.set();
    for (uint i=0; i!= result.size(); i++) {
        for (uint j=0;j!=props.size();++j)
            cout<<(pi[j]?'T':'F')<<"  ";
        cout<<(result[i]?'T':'F')<<endl;
        nextProp(pi);
    }

    // Print DNF and CNF.
    string dnf, cnf;
    tie(dnf, cnf) = computeNF(props, result);
    cout<<"The main DNF is:"<<endl<<dnf<<endl;
    cout<<"The main CNF is:"<<endl<<cnf<<endl;
    return 0;
}
