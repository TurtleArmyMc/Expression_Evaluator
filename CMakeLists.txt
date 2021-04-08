cmake_minimum_required(VERSION 3.12)
project(ExpressionEvaluator)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE 1oop_sources src/1.OOP/*.cpp src/1.OOP/*.hpp)
file(GLOB_RECURSE 2hybrid_sources src/2.HYBRID/*.cpp src/2.HYBRID/*.hpp)

add_executable(OOPExpressionEvaluator ${1oop_sources})
add_executable(HybridExpressionEvaluator ${2hybrid_sources})