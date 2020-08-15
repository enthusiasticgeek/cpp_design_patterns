cmake_minimum_required(VERSION 3.8)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
project(structural_design_pattern)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(Adapter Adapter.cpp)
add_executable(Bridge Bridge.cpp)
add_executable(Composite Composite.cpp)
add_executable(Decorator Decorator.cpp)
add_executable(Facade Facade.cpp)
add_executable(Proxy Proxy.cpp)
install(TARGETS 
	Adapter 
	Bridge 
	Composite 
	Decorator 
	Facade 
	Proxy 
	DESTINATION bin)
