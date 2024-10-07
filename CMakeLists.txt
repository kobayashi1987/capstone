cmake_minimum_required(VERSION 3.29)
project(project01)

set(CMAKE_CXX_STANDARD 20)

add_executable(project01 Customer.cpp
        Customer.h
        Main.cpp
        include/TradingStrategy.h
        src/TradingStrategy.cpp
        include/MovingAverageCrossoverStrategy.h
        src/MovingAverageCrossoverStrategy.cpp
        src/Main.cpp)


cmake_minimum_required(VERSION 3.16)
project(TradingAlgorithms)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(TradingAlgorithms ${SOURCES})