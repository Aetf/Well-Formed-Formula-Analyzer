#include "wffanalyzer.h"
#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;

// Now comes the core part of the algorithm!
//-1:calculate 1:push 0:pop(for brackets) -2:error
static int OPERATOR_WEIGHT[][8]={ {1,1,1,1,1,1,-1,1},
                            {-1,-1,-2,1,1,1,-1,1},
                            {-1,-2,-1,1,1,1,-1,1},
                            {-1,-1,-1,-2,-2,1,-1,1},
                            {-1,-1,-1,-2,-2,1,-1,1},
                            {1,1,1,1,1,1,-2,1},
                            {-1,-1,-1,-1,-1,0,-1,-2},
                            {-1,-1,-1,-1,-1,-2,-1,0} };

int operToIndex(string oper) // I used switch-case but only find that it does not support string.
{
    if(oper=="!")
        return 0;
    if(oper=="||")
        return 1;
    if(oper=="&&")
        return 2;
    if(oper=="<->")
        return 3;
    if(oper=="->")
        return 4;
    if(oper=="(")
        return 5;
    if(oper==")")
        return 6;
    if(oper=="#")
        return 7;
    return -1; // Not an operator.
}

int operToIndex(char oper) // For convenience.
{
    string temp(1,oper);
    return operToIndex(temp);
}

string cal(string prop1,string oper,string prop2) // Basic logic operation
{
    if(oper=="||")
        return ((prop1=="F")&&(prop2=="F")?"F":"T");
    if(oper=="&&")
        return ((prop1=="T")&&(prop2=="T")?"T":"F");
    if(oper=="->")
        return (prop1=="T"?prop2:"T");
    if(oper=="<->")
        return (prop1==prop2?"T":"F");
    return "X";
}

string cal(string prop1) // NOT needs only one variable.
{
    return prop1=="T"?"F":"T";
}

vector<string> strSplit(string _src)
{
    vector<string> result;
    string temp="";
    for (string::iterator cur=_src.begin(),next=cur+1;cur!=_src.end();++cur,++next)
    {
        temp+=*cur;
        if (operToIndex(temp)!=-1||isalpha(temp[0])) // Has got an operator or a proposition.
        {
            result.push_back(temp);
            temp.clear();
            continue;
        }
    }
    return result;
}

bool stackBasedCal(string exp)
{
    vector<string> splitExp;
    splitExp=strSplit(exp);  // Split the exp into divided propositions and operators.
    stack<string> optr,opnd; // Create operator stack and proposition stack.
    optr.push("#");
    vector<string>::iterator expIndex=splitExp.begin();
    string c=*expIndex;
    string oper1,prop1,oper,prop2;
    while (c!="#"||optr.top()!="#")
    {
        if (operToIndex(c)==-1) // It is a proposition
        {
            opnd.push(c);
            expIndex++;
            c=*expIndex;
        }
        else // It is a operator
        {
            oper1=optr.top();
            int oper1Index=operToIndex(oper1);
            int oper2Index=operToIndex(c);
            switch (OPERATOR_WEIGHT[oper2Index][oper1Index])
            {
                case 1:
                    optr.push(c);
                    expIndex++;
                    c=*expIndex;
                    break;
                case 0:
                    optr.pop();
                    expIndex++;
                    c=*expIndex;
                    break;
                case -1:
                    if(opnd.empty()) throw INVALID_EXP;
                    prop1=opnd.top();
                    opnd.pop();
                    oper=optr.top();
                    optr.pop();
                    if (operToIndex(oper)!=0) // It is a binary operator
                    {
                        if(opnd.empty()) throw INVALID_EXP;
                        prop2=opnd.top();
                        opnd.pop();
                        opnd.push(cal(prop2,oper,prop1)); // Calculate!
                    }
                    else
                    {
                        opnd.push(cal(prop1)); // Calculate!
                    }
                    break;
                case -2: //There is something wrong!
                    throw INVALID_EXP;
                default:
                    break;
            }
        }
    }
    string temp=opnd.top();
    opnd.pop();
    if (!opnd.empty()) // After all the operations, opnd stack is not empty only means invalid expression.
        throw INVALID_EXP;
    return temp=="T"; // Transform string to bool.
}

// find all propositions and return maximum configuration number
uint64_t countProp(string exp,vector<string>& props)
{
    for(string::iterator i=exp.begin();i!=exp.end();++i)
        if (isalpha(*i))
            props.push_back(exp.substr(i-exp.begin(),1));
        sort(props.begin(),props.end());

    // Exclude duplicate elements.
    vector<string>::iterator unq = unique(props.begin(), props.end());
    props.erase(unq, props.end());

    // Reduce the memory usage.
    vector<string> temp(props);
    temp.swap(props);

    return 2ul << (props.size()-1);
}

void stringReplace(string &strBase, const string &strSrc, const string &strDes) // As the name shows.
{
    string::size_type pos = 0;
    string::size_type srcLen = strSrc.size();
    string::size_type desLen = strDes.size();
    pos=strBase.find(strSrc, pos);
    while ((pos != string::npos))
    {
        strBase.replace(pos, srcLen, strDes);
        pos=strBase.find(strSrc, (pos+desLen));
    }
}

string performP(string exp, const vector<string>& props, uint64_t configuration)
{
    for(uint i = 0; i!= props.size(); i++)
        stringReplace(exp, props.at(i),
                      assignmentAt(configuration, i) ? "T" : "F");
    return exp;
}

bool assignmentAt(uint64_t configuration, uint varPosition)
{
    return (configuration & (1 << varPosition));
}

bool analyzeExpression(const string &expr, vector<string> &props, vector<bool> &results)
{
    // find all proposition variables
    uint64_t maxResult = countProp(expr, props);

    // save the result of WFF.
    results.clear();
    results.reserve(maxResult);

    // check and calculate.
    try {
        // loop through all possible configurations
        for(uint64_t conf = 0; conf!= maxResult; conf++) {
            // add a # to mark the end as required by stackBasedCal
            results.push_back(stackBasedCal(performP(expr, props, conf) + "#"));
        }
    } catch (int) {
        return false;
    }

    return true;
}

pair<string, string> computeNF(const vector<string> &props, const vector<bool> &results)
{
    string dnf="",cnf="";
    vector<string> dnfv, cnfv, term;
    term.reserve(props.size());
    for (uint64_t conf = 0; conf!= results.size(); conf++) {
        term.clear();
        if (results[conf]) {
            for (uint i = 0; i!= props.size(); i++) {
                term.push_back(assignmentAt(conf, i) ?
                                   props[i]
                                   : "!" + props[i]);
            }
            dnfv.push_back("(" + join(term, "&&") + ")");
        } else {
            for (uint i = 0; i!= props.size(); i++) {
                term.push_back(!assignmentAt(conf, i) ?
                                   props[i]
                                   : "!" + props[i]);
            }
            cnfv.push_back("(" + join(term, "||") + ")");
        }
    }
    dnf = join(dnfv, "||");
    cnf = join(cnfv, "&&");
    // Remove the brackets if there's only one term.
    if(dnfv.size() == 1)
        dnf = dnf.substr(1,dnf.size()-2);
    if(cnfv.size() == 1)
        cnf = cnf.substr(1,cnf.size()-2);

    return {dnf, cnf};
}
