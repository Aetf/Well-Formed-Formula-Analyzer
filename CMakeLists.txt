cmake_minimum_required(VERSION 2.8)

project(WFFAnalyzer)

# Include path
include_directories($(WFFAnalyzer_SOURCE_DIR)/include)
# Compiler Options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(wffanalyze 
            src/main.cpp
            src/wffanalyzer.cpp)

add_subdirectory(tests)