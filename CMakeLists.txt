cmake_minimum_required(VERSION 3.29)
project(project01)

set(CMAKE_CXX_STANDARD 20)

add_executable(project01 Customer.cpp
        Customer.h
        Main.cpp)
