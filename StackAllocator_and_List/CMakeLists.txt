cmake_minimum_required(VERSION 3.0.0)
project(StackAlloctor_and_List)

set(CMAKE_CXX_STANDARD 14)

include_directories(StackAllocator_and_List)

add_executable(StackAllocator
        StackAllocator.h
        List.h
        example.cpp)