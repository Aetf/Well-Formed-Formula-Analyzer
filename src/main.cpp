#include <iostream>
#include <bitset>
#include <algorithm>

#include "wffanalyzer.h"

using namespace std;

int main(int argc,char* argv[])
{
    if (argc!=2)
    {
        cout<<"Invalid arguments!"<<endl<<"Usage:"<<endl<<argv[0]<<" '[WFF]'"<<endl;
        return -1;
    }

    // Get the WFF from argument. And add a # to mark the end.
    string exp(argv[1]);
    exp+='#';

    // props saves the propositions which will be used below.
    vector<string> props;

    // find all proposition variables
    uint maxResult = countProp(exp, props);

    // save the result of WFF.
    int *result = new int[maxResult];

    // pi saves the assignment for the propositions, all of them are F first.
    bitset<MAX_PROP_VARIABLE> pi;
    pi.set();

    // Now let's begin!
    // Check if the WFF is valid, and calculate.
    try
    {
        for(uint i=0;i!=maxResult;++i)
        {
            result[i]=stackBasedCal(performP(exp,props,pi));
            nextProp(pi);
        }
    }
    catch (int)
    {
        cout<<"Invalid expression!"<<endl;
        delete [] result;
        return -1;
    }

    // The WFF has been checked, and all the result has been save to result array.
    // Print table header.
    cout<<"The expression is valid. Lay out the truth table:"<<endl;
    for (auto i=props.begin();i!=props.end();++i)
        cout<<*i<<"  ";
    cout<<exp.substr(0,exp.size()-1)<<endl;

    // Print truth table
    pi.set();
    for (uint i=0;i!=maxResult;++i)
    {
        for (uint j=0;j!=props.size();++j)
            cout<<(pi[j]?'T':'F')<<"  ";
        cout<<(result[i]?'T':'F')<<endl;
        nextProp(pi);
    }

    // Print DNF and CNF.
    pi.set();
    string dnf="",cnf="";
    uint truthNum=0;
    for(uint i=0;i!=maxResult;++i) {
        if (result[i]) {
            ++truthNum;
            dnf+="(";
            for(uint j=0;j!=propNum;++j) {
                if (!pi[j])
                    dnf+="!";
                dnf+=props.at(j);
                dnf+="&&";
            }
            dnf.erase(dnf.size()-2,2); // Cut the last two char in DNF and CNF.
            dnf+=")||";
        } else {
            cnf+="(";
            for(uint j=0;j!=propNum;++j) {
                if (pi[j])
                    cnf+="!";
                cnf+=props.at(j);
                cnf+="||";
            }
            cnf.erase(cnf.size()-2,2); // Cut the last two char in DNF and CNF.
            cnf+=")&&";
        }
        nextProp(pi);
    }

    if(truthNum==1)  // Remove the brackets if there's only one term.
        dnf=dnf.substr(1,dnf.size()-2);
    if(maxResult-truthNum==1)
        cnf=cnf.substr(1,cnf.size()-2);
    dnf.erase(dnf.size()-2,2); // Cut the last two char in DNF and CNF.
    cnf.erase(cnf.size()-2,2); // Cut the last two char in DNF and CNF.
    cout<<"The main DNF is:"<<endl<<dnf<<endl;
    cout<<"The main CNF is:"<<endl<<cnf<<endl;
    delete [] result;
    return 0;
}
