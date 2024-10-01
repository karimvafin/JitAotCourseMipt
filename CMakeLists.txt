cmake_minimum_required(VERSION 3.20)
project(JitAotCourseMipt)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_graph_factorial main.cpp BasicBlock.cpp Graph.cpp Instruction.cpp)
