cmake_minimum_required(VERSION 2.8)

find_library(GTEST_LIB gtest)

add_executable(wffanalyzertest wffanalyzertest.cpp ../src/wffanalyzer.cpp)
target_link_libraries(wffanalyzertest ${GTEST_LIB})

add_test(WffanalyzeTest ${EXECUTABLE_OUTPUT_PATH}/wffanalyze)