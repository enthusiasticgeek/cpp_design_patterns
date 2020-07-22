cmake_minimum_required(VERSION 3.8)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
project(creational_design_pattern)
add_executable(Factory Factory.cpp)
add_executable(AbstractFactory AbstractFactory.cpp)
install(TARGETS 
	Factory 
	AbstractFactory 
	DESTINATION bin)
