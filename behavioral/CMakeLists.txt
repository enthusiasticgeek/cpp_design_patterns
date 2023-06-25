cmake_minimum_required(VERSION 3.8)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
project(behavioral_design_pattern)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(ChainOfResponsibility ChainOfResponsibility.cpp)
add_executable(Command Command.cpp)
add_executable(Iterator Iterator.cpp)
add_executable(Mediator Mediator.cpp)
add_executable(Memento Memento.cpp)
add_executable(Observer Observer.cpp)
add_executable(State State.cpp)
add_executable(Strategy Strategy.cpp)
add_executable(Template Template.cpp)
add_executable(Visitor Visitor.cpp)
install(TARGETS
 	ChainOfResponsibility
  	Command
        Iterator
	Mediator
	Memento
 	Observer
  	State
   	Strategy
    	Template
     	Visitor
	DESTINATION bin)
