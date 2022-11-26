cmake_minimum_required(VERSION 3.0.0)
project(BigInteger)

set(CMAKE_CXX_STANDARD 14)

include_directories(BigInteger)

add_executable(BigInteger
        biginteger.h
        example.cpp
        rational.h)
