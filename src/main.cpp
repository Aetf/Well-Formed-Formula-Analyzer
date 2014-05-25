#include <iostream>
#include <cmath>
#include <cctype>
#include <bitset>
#include <stack>
#include <algorithm>

#include "wffanalyzer.h"

using namespace std;

int main(int argc,char* argv[])
{
    if (argc!=2)
    {
        cout<<"Parameter invalid!"<<endl<<"Usage:"<<endl<<argv[0]<<" '[WFF]'"<<endl;
        return -1;
    }
    string exp(argv[1]);
//    string exp("P||Q");
    exp+='#';  // Get the WFF from the console. And add a # to mark the end.
    vector<string> props;
    size_t propNum=0;
    propNum=countProp(exp,props); // How many different propositions?
                                  // props saves the propositions which will be used below.
    int maxResult=pow(2,propNum); // There are pow(2,propNum) kinds of different situations.
    int *result;
    result=new int[maxResult]; // save the result of WFF.
    bitset<maxProp> pi;
    pi.set(); // pi saves the T/F for the propositions, all of them are F first.

    // Now let's begin!
    // Check if the WFF is valid, and calculate.
    try
    {
        for(int i=0;i!=maxResult;++i)
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
    // Print the truth table.
    cout<<"The expression is valid. Lay out the truth table:"<<endl;
    for (vector<string>::iterator i=props.begin();i!=props.end();++i)
        cout<<*i<<"  ";
    cout<<exp.substr(0,exp.size()-1)<<endl;
    pi.set();
    for (int i=0;i!=maxResult;++i)
    {
        for (unsigned int j=0;j!=propNum;++j)
            cout<<(pi[j]?'T':'F')<<"  ";
        cout<<(result[i]?'T':'F')<<endl;
        nextProp(pi);
    }
    // Lay out the main DNF and CNF.
    pi.set();
    string dnf="",cnf="";
    int truthNum=0;
    for(int i=0;i!=maxResult;++i)
    {
        if (result[i])
        {
            ++truthNum;
            dnf+="(";
            for(unsigned int j=0;j!=propNum;++j)
            {
                if (!pi[j])
                    dnf+="!";
                dnf+=props.at(j);
                dnf+="&&";
            }
            dnf.erase(dnf.size()-2,2); // Cut the last two char in DNF and CNF.
            dnf+=")||";
        }else
        {
            cnf+="(";
            for(unsigned int j=0;j!=propNum;++j)
            {
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