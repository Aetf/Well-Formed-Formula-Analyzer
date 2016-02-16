#include "wffanalyzer.h"
#include "gtest/gtest.h"

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
    
    // Tests that split expression works.
    TEST_F(WFFAnalyzerTest, ExprSplit) {
        const string expr = "P||Q";
        vector<string> tokens;
        tokens.push_back("P");
        tokens.push_back("||");
        tokens.push_back("Q");
        vector<string> res = strSplit(expr);
        ASSERT_EQ(tokens.size(), res.size()) << "strSplit returns wrong number of tokens";
        
        for(unsigned i = 0; i!= tokens.size(); i++)
        {
            EXPECT_EQ(tokens[i], res[i]) << "Vectors tokens and res differ at index " << i;
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
        vector<string> props, res;
        props.push_back("P");
        props.push_back("Q");
        props.push_back("R");

        auto assignCount = countProp(expr, res);
        
        for(unsigned i = 0; i!= props.size(); i++)
        {
            EXPECT_EQ(props[i], res[i]) << "Vectors props and res differ at index " << i;
        }

        ASSERT_EQ(8u, assignCount) << "countProp returns wrong number of assignments";
    }
    
    // Tests that string replace performs correct.
    TEST_F(WFFAnalyzerTest, StrReplace) {
        string str = "Hello world!";
        string src = "Hello";
        string dest = "Hi";
        stringReplace(str, src, dest);
        ASSERT_EQ("Hi world!", str);
    }
    
    // Tests that substitution propositions works correct.
    TEST_F(WFFAnalyzerTest, PerformProp) {
        const string expr = "((P||Q)&&R)->P";
        vector<string> props;
        props.push_back("P");
        props.push_back("Q");
        props.push_back("R");
        bitset<MAX_PROP_VARIABLE> pi;
        pi.set(0, false).set(1).set(2);

        ASSERT_EQ("((F||T)&&T)->F", performP(expr, props, pi));
    }
    
    // Tests that operator to index conversion correct.
    TEST_F(WFFAnalyzerTest, NextProps) {
        bitset<MAX_PROP_VARIABLE> pi, nxt;
        pi.set(0, false).set(1).set(2);
        nxt.set(0).set(1, false).set(2);
        nextProp(pi);
        
        ASSERT_EQ(nxt, pi);
    }

}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
