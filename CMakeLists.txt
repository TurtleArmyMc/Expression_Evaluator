cmake_minimum_required(VERSION 3.12)
project(ExpressionEvaluator)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE sources src/*.cpp src/*.hpp)

add_executable(ExpressionEvaluator ${sources})