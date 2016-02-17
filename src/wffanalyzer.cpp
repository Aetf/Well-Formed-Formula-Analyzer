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
    if(oper == "!")
        return 0;
    if(oper == "||")
        return 1;
    if(oper == "&&")
        return 2;
    if(oper == "<->")
        return 3;
    if(oper == "->")
        return 4;
    if(oper == "(")
        return 5;
    if(oper == ")")
        return 6;
    if(oper == "#")
        return 7;
    return -1; // Not an operator.
}

int operToIndex(char oper) // For convenience.
{
    return operToIndex(string(1, oper));
}

string cal(string prop1,string oper,string prop2) // Basic logic operation
{
    if(oper == "||")
        return ((prop1 == "F") && (prop2 == "F") ? "F" : "T");
    if(oper == "&&")
        return ((prop1 == "T") && (prop2 == "T") ? "T" : "F");
    if(oper == "->")
        return (prop1 == "T" ? prop2 : "T");
    if(oper == "<->")
        return (prop1 == prop2 ? "T" : "F");
    return "X";
}

string cal(string prop1) // NOT needs only one variable.
{
    return prop1 == "T" ? "F" : "T";
}

vector<string> tokenize(const string &src)
{
    vector<string> tokens;
    string temp;
    for (auto cur : src) {
        temp += cur;
        if (operToIndex(temp) != -1 || isalpha(temp[0])) {
            // Has got an operator or a proposition.
            tokens.push_back(temp);
            temp.clear();
        }
    }
    return tokens;
}

bool stackBasedCal(const vector<string> &exp)
{
    // add a # to mark the end as required by stackBasedCal
    auto tokens = exp;
    tokens.push_back("#");

    // Create operator stack and variable stack.
    stack<string> optr, opnd;
    optr.push("#");

    auto expIter = tokens.begin();
    while (expIter != tokens.end()
           && (*expIter != "#" || optr.top() != "#")) {
        int curOperIndex = operToIndex(*expIter);
        if (curOperIndex == -1) {
            // It is a variable
            opnd.push(*expIter++);
        } else {
            // It is a operator
            if (optr.empty())
                throw INVALID_EXP;

            auto oper = optr.top();
            int topOperIndex = operToIndex(oper);
            switch (OPERATOR_WEIGHT[curOperIndex][topOperIndex])
            {
            case 1:
                optr.push(*expIter++);
                break;
            case 0:
                optr.pop();
                expIter++;
                break;
            case -1:
                optr.pop();
                if (topOperIndex != 0) {
                    // It is a binary operator
                    if(opnd.size() < 2)
                        throw INVALID_EXP;

                    string prop1, prop2;
                    prop1 = opnd.top(); opnd.pop();
                    prop2 = opnd.top(); opnd.pop();
                    opnd.push(cal(prop2, oper, prop1)); // Calculate!
                } else {
                    if(opnd.size() < 1)
                        throw INVALID_EXP;

                    string prop1;
                    prop1 = opnd.top(); opnd.pop();
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
    // After all the operations, opnd stack should have exactly 1 element
    // and optr stack should only has end mark in it
    if (opnd.size() == 1 && optr.size() == 1 && optr.top() == "#")
        return opnd.top() == "T";
    throw INVALID_EXP;
}

// find all propositions and return maximum configuration number
uint64_t countProp(const vector<string> &tokens, vector<string>& props)
{
    props.clear();
    for (auto token : tokens) {
        if (operToIndex(token) == -1)
            props.push_back(token);
    }

    sort(props.begin(), props.end());
    // Exclude duplicate elements.
    auto unq = unique(props.begin(), props.end());
    props.erase(unq, props.end());

    // Reduce the memory usage.
    vector<string> temp(props);
    temp.swap(props);

    return 2ul << (props.size()-1);
}

void tokenReplace(vector<string> &strBase, const string &strSrc, const string &strDes)
{
    for (auto &token : strBase) {
        if (token == strSrc)
            token = strDes;
    }
}

vector<string> performP(const vector<string> &tokens, const vector<string>& props, uint64_t configuration)
{
    auto res = tokens;
    for(uint i = 0; i != props.size(); i++)
        tokenReplace(res, props.at(i),
                      assignmentAt(configuration, i) ? "T" : "F");
    return res;
}

bool assignmentAt(uint64_t configuration, uint varPosition)
{
    return (configuration & (1 << varPosition));
}

bool analyzeExpression(const string &expr, vector<string> &props, vector<bool> &results)
{
    // first tokenize
    auto tokens = tokenize(expr);
    // find all proposition variables
    auto maxConf = countProp(tokens, props);

    // save the result of WFF.
    results.clear();
    results.reserve(maxConf);

    // check and calculate.
    try {
        // loop through all possible configurations
        for(uint64_t conf = 0; conf != maxConf; conf++) {
            results.push_back(stackBasedCal(performP(tokens, props, conf)));
        }
    } catch (int) {
        return false;
    }

    return true;
}

pair<string, string> computeNF(const vector<string> &props, const vector<bool> &results)
{
    vector<string> dnfv, cnfv, term;
    term.reserve(props.size());
    for (uint64_t conf = 0; conf != results.size(); conf++) {
        term.clear();
        if (results[conf]) {
            for (uint i = 0; i != props.size(); i++) {
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

    auto dnf = join(dnfv, "||");
    auto cnf = join(cnfv, "&&");
    // Remove the brackets if there's only one term.
    if(dnfv.size() == 1)
        dnf = dnf.substr(1,dnf.size()-2);
    if(cnfv.size() == 1)
        cnf = cnf.substr(1,cnf.size()-2);

    return {dnf, cnf};
}
