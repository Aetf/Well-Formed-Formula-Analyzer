#include <iostream>
#include <cmath>
#include <string>
#include <cctype>
#include <vector>
#include <bitset>
#include <stack>
#include <algorithm>

using namespace std;

const int maxProp=32; // No more than 10 propositions can be calculated.
const int INVALID_EXP=-1;

// Now comes the core part of the algorithm!
int OPERATOR_WEIGHT[][8]={ {1,1,1,1,1,1,-1,1},
                          {-1,-1,-2,1,1,1,-1,1},
                          {-1,-2,-1,1,1,1,-1,1},
                          {-1,-1,-1,-2,-2,1,-1,1},
                          {-1,-1,-1,-2,-2,1,-1,1},
                          {1,1,1,1,1,1,-2,1},
                          {-1,-1,-1,-1,-1,0,-1,-2},
                          {-1,-1,-1,-1,-1,-2,-1,0} };  //-1:calculate 1:push 0:pop(for brackets) -2:error

int operToIndex(string oper);
int operToIndex(char oper);
string cal(string prop1,string oper,string prop2);
string cal(string prop1);
vector<string> strSplit(string _src);
bool stackBasedCal(string exp);
int countProp(string exp,vector<string>& props);
void stringReplace(string &strBase, const string &strSrc, const string &strDes);
string performP(string exp,const vector<string>& props,const bitset<maxProp> pi);
bitset<maxProp>& nextProp(bitset<maxProp>& p);

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
                break;
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

int countProp(string exp,vector<string>& props) // Count propositions and save a copy to prop
{
    for(string::iterator i=exp.begin();i!=exp.end();++i)
        if (isalpha(*i))
            props.push_back(exp.substr(i-exp.begin(),1));
    sort(props.begin(),props.end());
    vector<string>::iterator unq=unique(props.begin(),props.end()); // Exclude duplicate elements.
    props.erase(unq,props.end());
    vector<string> temp(props);
    temp.swap(props); // Reduce the memory usage.
    return props.size();
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

string performP(string exp,const vector<string>& props,const bitset<maxProp> pi)
{
    for(unsigned int i=0;i!=props.size();++i)
        stringReplace(exp,props.at(i),(pi[i]?"T":"F"));
    return exp;
}

bitset<maxProp>& nextProp(bitset<maxProp>& p) // This traverses all the assignments. I did not use loop to do this for the number of propositions is unknown.
{
    bitset<maxProp> t(p.to_ulong()-1);
    p=t;
    return p;
}
