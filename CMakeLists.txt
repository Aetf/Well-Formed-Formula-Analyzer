cmake_minimum_required(VERSION 2.8)

project(WFFAnalyzer)

# Include custom cmake modules
include(cmake/Utilities.cmake)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Ensure out-of-source build
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Usage: mkdir build; cmake ..")
endif()

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Use C++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add some basic compiler flags
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    # using Clang or AppleClang

    # reasonable and standard
    add_compile_options_with_check(-Wall)
    add_compile_options_with_check(-Wextra)
    add_compile_options_with_check(-pedantic)
    add_compile_options_with_check(-Werror)

    add_compile_options_with_check(-Wno-c++98-compat)

    add_compile_options_with_check(-Wold-style-cast)
    # helps catch hard to track down memory errors
    add_compile_options_with_check(-Wnon-virtual-dtor)
    # warn for potential performance problem casts
    add_compile_options_with_check(-Wcast-align)
    # warn if you overload (not override) a virtual function
    add_compile_options_with_check(-Woverloaded-virtual)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC

    # reasonable and standard
    add_compile_options_with_check(-Wall)
    add_compile_options_with_check(-Wextra)
    add_compile_options_with_check(-pedantic)
    add_compile_options_with_check(-Werror)

    add_compile_options_with_check(-Wold-style-cast)
    # helps catch hard to track down memory errors
    add_compile_options_with_check(-Wnon-virtual-dtor)
    # warn for potential performance problem casts
    add_compile_options_with_check(-Wcast-align)
    # warn if you overload (not override) a virtual function
    add_compile_options_with_check(-Woverloaded-virtual)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++

    # all reasonable warnings
    add_compile_options_with_check(/W4)
    # treat warnings as errors
    add_compile_options_with_check(/Wx)
    # enable warning on thread un-safe static member initialization
    add_compile_options_with_check(/W44640)
endif()

# add public include path
include_directories(include)

add_executable(wffanalyze 
            src/main.cpp
            src/wffanalyzer.cpp)

add_subdirectory(tests)
