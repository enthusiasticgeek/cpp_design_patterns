cmake_minimum_required(VERSION 3.8)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
project(creational_design_pattern)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(Adapter Adapter.cpp)
install(TARGETS 
	Adapter 
	DESTINATION bin)
