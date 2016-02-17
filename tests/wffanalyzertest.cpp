#include "wffanalyzer.h"
#include "gtest/gtest.h"
#include <bitset>
using std::bitset;

namespace {
    
    // The fixture for testing class WFFAnalyzer.
    class WFFAnalyzerTest : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.
        
        WFFAnalyzerTest() {
            // You can do set-up work for each test here.
        }
        
        virtual ~WFFAnalyzerTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }
        
        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:
        
        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }
        
        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }
        
        // Objects declared here can be used by all tests in the test case for Foo.
    };
       
    // Tests that operator to index conversion correct.
    TEST_F(WFFAnalyzerTest, OpToIdxStr) {
        const string input_op = "||";
        const int output_idx = 1;
        ASSERT_EQ(output_idx, operToIndex(input_op)) << "failed on operator " << input_op;
    }
    
    // Tests that operator to index conversion correct.
    TEST_F(WFFAnalyzerTest, OpToIdxCh) {
        const char input_op = '!';
        const int output_idx = 0;
        ASSERT_EQ(output_idx, operToIndex(input_op)) << "failed on operator " << input_op;
    }
    
    // Tests that basic calculation on binary operator correct.
    TEST_F(WFFAnalyzerTest, BasicOpCalculationBin) {
        const string para1 = "T";
        const string para2 = "F";
        const string op = "&&";
        ASSERT_EQ("F", cal(para1, op, para2)) << "failed on " 
                                              << para1 << " " << op << " " << para2;
    }
    
    // Tests that basic calculation on not operator correct.
    TEST_F(WFFAnalyzerTest, BasicOpCalculationUni) {
        const string para = "T";
        ASSERT_EQ("F", cal(para)) << "failed on !" << para;
    }
    
    // Tests that expression tokenization works.
    TEST_F(WFFAnalyzerTest, ExprTokenization) {
        vector<string> tokens{"P", "||", "Q"};
        auto res = tokenize("P||Q");

        ASSERT_EQ(tokens.size(), res.size())
                << "strSplit returns wrong number of tokens";
        for(uint i = 0; i!= tokens.size(); i++)
        {
            EXPECT_EQ(tokens[i], res[i])
                    << "Vectors tokens and res differ at index " << i;
        }
    }
    
    // Tests that stack based calculation correct.
    TEST_F(WFFAnalyzerTest, StackBasedCal) {
        const string expr = "((F||T)&&T)->F#";
        ASSERT_FALSE(stackBasedCal(expr)) << "Wrong result on expr " << expr;
    }
    
    // Tests that propositions in expr can be count correct.
    TEST_F(WFFAnalyzerTest, CountPropInExpr) {
        const string expr = "((P||Q)&&R)->P";
        vector<string> props{"P", "Q", "R"};
        vector<string> res;

        auto assignCount = countProp(expr, res);
        
        for(uint i = 0; i!= props.size(); i++)
        {
            EXPECT_EQ(props[i], res[i]) << "Vectors props and res differ at index " << i;
        }

        ASSERT_EQ(8u, assignCount) << "countProp returns wrong number of assignments";
    }
    
    // Tests that string replace performs correct.
    TEST_F(WFFAnalyzerTest, StrReplace) {
        string str = "HeHelloHelloHellHelloHe";
        stringReplace(str, "Hello", "Hi");
        ASSERT_EQ("HeHiHiHellHiHe", str);
    }
    
    // Tests that substitution propositions works correct.
    TEST_F(WFFAnalyzerTest, PerformProp) {
        ASSERT_EQ("((F||T)&&T)->F",
                  performP("((P||Q)&&R)->P",
                           {"P", "Q", "R"},
                           0b110));
    }
    
    TEST_F(WFFAnalyzerTest, EmptyDNF) {
        vector<string> props({"P", "Q", "R"});
        vector<bool> res{false, false, false, false,
                          false, false, false, false};

        string dnf, cnf;
        tie(dnf, cnf) = computeNF(props, res);
        ASSERT_EQ("", dnf);
        ASSERT_EQ("(P||Q||R)&&(!P||Q||R)&&(P||!Q||R)&&(!P||!Q||R)&&(P||Q||!R)&&(!P||Q||!R)&&(P||!Q||!R)&&(!P||!Q||!R)",
                  cnf);
    }

    TEST_F(WFFAnalyzerTest, EmptyCNF) {
        string dnf, cnf;
        tie(dnf, cnf) = computeNF({"P", "Q"}, {true, true, true, true});
        ASSERT_EQ("(!P&&!Q)||(P&&!Q)||(!P&&Q)||(P&&Q)", dnf);
        ASSERT_EQ("", cnf);
    }

    TEST_F(WFFAnalyzerTest, AssignmentForWork) {
        bitset<MAX_PROP_VARIABLE> pi;
        pi.reset();
        uint max_prop = 20;
        for (uint64_t i = 0; i!= 2ul << (max_prop-1); i++) {
            for (uint j = 0; j!= max_prop; j++) {
                EXPECT_EQ(pi[j], assignmentAt(i, j))
                        << "Wrong assignment for variable " << j
                        << " at assignment No. " << i;
            }
            pi = {pi.to_ullong() + 1};
        }
    }
}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
