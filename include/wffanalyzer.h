#ifndef WFFANALYZER_H
#define WFFANALYZER_H

#include <string>
#include <vector>
#include <utility>
#include <sstream>
using std::string;
using std::vector;
using std::pair;
using std::ostringstream;

const int MAX_PROP_VARIABLE=32; // No more than 10 propositions can be calculated.
const int INVALID_EXP=-1;

template <typename T>
string join(const T& v, const string& delim) {
    std::ostringstream s;
    for (const auto& i : v) {
        if (&i != &v[0]) {
            s << delim;
        }
        s << i;
    }
    return s.str();
}

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
vector<string> tokenize(const string &src);

/**
 * Do calculation on the exp.
 * return the result
 */
bool stackBasedCal(const vector<string> &exp);

/**
 * Count the proposition number in a given expression,
 * returns maximum configuration number.
 */
uint64_t countProp(const vector<string> &tokens, vector<string>& props);

/**
 * Replace part of a token stream
 */
void tokenReplace(vector<string> &strBase, const string &strSrc, const string &strDes);

/**
 * Substitude the propositions according to pi.
 */
vector<string> performP(const vector<string> &tokens, const vector<string>& props, uint64_t configuration);

/**
 * @brief assignment for variable at position, in configuration
 * @param configuration
 * @param varPosition
 * @return the assignment
 */
bool assignmentAt(uint64_t configuration, uint varPosition);

/**
 * @brief Analyze the expression expr, compute it for all possible assignment of variables in expr.
 * @param expr the expression to compute
 * @param props extracted propositional variable list
 * @param results computed results
 * @return if the expression is valid
 */
bool analyzeExpression(const string &expr, vector<string> &props, vector<bool> &results);

/**
 * @brief generate CNF and DNF from results
 * @param props all propositional variables
 * @param results results got from analyzeExpression
 * @return a pair of string (DNF, CNF)
 */
pair<string, string> computeNF(const vector<string> &props, const vector<bool> &results);

#endif // WFFANALYZER_H
