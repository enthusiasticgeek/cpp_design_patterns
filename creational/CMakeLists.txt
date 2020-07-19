cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_BUILD_TYPE Debug)
# set the C++17 standard
set(CMAKE_CXX_STANDARD 17)
project(creational_design_pattern)
add_executable(Factory Factory.cpp)
install(TARGETS Factory DESTINATION bin)
