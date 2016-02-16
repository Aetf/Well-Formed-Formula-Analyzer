#ifndef WFFANALYZER_H
#define WFFANALYZER_H

#include <string>
#include <vector>
#include <bitset>
using std::string;
using std::vector;
using std::bitset;

const int MAX_PROP_VARIABLE=32; // No more than 10 propositions can be calculated.
const int INVALID_EXP=-1;

/**
 * Convert a operator string to its index
 */
int operToIndex(string oper);

/**
 * Convert a operator char to its index
 */
int operToIndex(char oper);

/**
 * Do calculation with binary operator
 */
string cal(string prop1,string oper,string prop2);

/**
 * Do 'NOT' calculation
 */
string cal(string prop1);

/**
 * Split a string into vector, seperated by oeprators
 */
vector<string> strSplit(string _src);

/**
 * Do calculation on the exp.
 * return the result
 */
bool stackBasedCal(string exp);

/**
 * Count the proposition number in a given expression,
 * returns assignment count.
 */
uint countProp(string exp,vector<string>& props);

/**
 * Replace a part of string
 */
void stringReplace(string &strBase, const string &strSrc, const string &strDes);

/**
 * Substitude the propositions according to pi.
 */
string performP(string exp,const vector<string>& props,const bitset<MAX_PROP_VARIABLE> &pi);

/**
 * Generate next bitset
 */
bitset<MAX_PROP_VARIABLE>& nextProp(bitset<MAX_PROP_VARIABLE>& p);

#endif // WFFANALYZER_H
