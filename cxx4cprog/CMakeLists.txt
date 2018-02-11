cmake_minimum_required (VERSION 3.0)

project (cxx4cprog)

include_directories(.)

set (CMAKE_CXX_FLAGS "-std=c++1z")

add_executable(run.app
    graph.h
    graph.cc
    main.cc
)
