cmake_minimum_required(VERSION 3.10)
project(TradingApplication)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Gather all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable
add_executable(TradingApplication ${SOURCES})

# Link libraries if necessary (e.g., pthread)
find_package(Threads REQUIRED)
target_link_libraries(TradingApplication PRIVATE Threads::Threads)

# For JNI integration (if applicable)
find_package(JNI REQUIRED)
if (JNI_FOUND)
    message(STATUS "JNI found at ${JNI_INCLUDE_DIRS}")
    include_directories(${JNI_INCLUDE_DIRS})
    # If building a shared library for JNI
    # add_library(TradingEngineNative SHARED ${SOURCES})
    # target_link_libraries(TradingEngineNative PRIVATE Threads::Threads)
endif()

# Additional compiler options (optional)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(TradingApplication PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(TradingApplication PRIVATE /W4)
endif()