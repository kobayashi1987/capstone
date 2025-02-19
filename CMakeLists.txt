#cmake_minimum_required(VERSION 3.10)
#project(TradingApplication)
#
## Set the C++ standard
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
## Include directories
#include_directories(include)
#
## Gather all source files
#file(GLOB_RECURSE SOURCES "src/*.cpp")
#
## Create the executable
#add_executable(TradingApplication ${SOURCES}
#        include/Order.h
#        src/Order.cpp
#        include/Trade.h
#        src/Trade.cpp
#        include/OrderExecutor.h
#        src/OrderExecutor.cpp
#        include/Portfolio.h
#        src/Portfolio.cpp
#        include/TradingEngine.h
#        src/TradingEngine.cpp
#        include/MarketDataFeed.h
#        src/MarketDataFeed.cpp
#        src/main.cpp
#        include/MomentumStrategy.h
#        src/MomentumStrategy.cpp
#        include/BreakoutStrategy.h
#        src/BreakoutStrategy.cpp
#        include/MeanReversionStrategy.h
#        src/MeanReversionStrategy.cpp)
#
## Link libraries if necessary (e.g., pthread)
#find_package(Threads REQUIRED)
#target_link_libraries(TradingApplication PRIVATE Threads::Threads)
#
## For JNI integration (if applicable)
#find_package(JNI REQUIRED)
#if (JNI_FOUND)
#    message(STATUS "JNI found at ${JNI_INCLUDE_DIRS}")
#    include_directories(${JNI_INCLUDE_DIRS})
#    # If building a shared library for JNI
#    # add_library(TradingEngineNative SHARED ${SOURCES})
#    # target_link_libraries(TradingEngineNative PRIVATE Threads::Threads)
#endif()
#
## Additional compiler options (optional)
#if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#    target_compile_options(TradingApplication PRIVATE -Wall -Wextra -Wpedantic)
#elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    target_compile_options(TradingApplication PRIVATE /W4)
#endif()


# code added on 2024.10.23

#CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(TradingApplication)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create variables for source files
set(SOURCE_FILES
        src/main.cpp
        src/Order.cpp
        src/Trade.cpp
        src/OrderExecutor.cpp
        src/Portfolio.cpp
        src/TradingEngine.cpp
        src/TradingStrategy.cpp
        src/MarketDataFeed.cpp
        src/MovingAverageCrossoverStrategy.cpp
        src/RSIStrategy.cpp
        src/MomentumStrategy.cpp
        src/BreakoutStrategy.cpp
        src/MeanReversionStrategy.cpp
        src/DataPersistence.cpp
        src/StrategyManager.cpp
        src/StrategyManager.cpp
        src/Utils.cpp
        src/BollingerBandsStrategy.cpp

)

# Create variables for header files (optional, for IDEs)
set(HEADER_FILES
        include/Order.h
        include/Trade.h
        include/OrderExecutor.h
        include/Portfolio.h
        include/TradingEngine.h
        include/TradingStrategy.h
        include/MarketDataFeed.h
        include/MovingAverageCrossoverStrategy.h
        include/RSIStrategy.h
        include/MomentumStrategy.h
        include/BreakoutStrategy.h
        include/MeanReversionStrategy.h
        include/DataPersistence.h
        include/StrategyManager.h
        include/Utils.h
        include/Utils.h
        include/ProfitLoss.h
        include/BollingerBandsStrategy.h
        include/BollingerBandsStrategy.h

)

# Add the executable target
add_executable(TradingApplication ${SOURCE_FILES} ${HEADER_FILES}
        src/DataPersistence.cpp
        src/StrategyManager.cpp)

# Set include directories for the target
target_include_directories(TradingApplication PRIVATE include)

# Find and link libraries if necessary (e.g., pthread)
find_package(Threads REQUIRED)
target_link_libraries(TradingApplication PRIVATE Threads::Threads)

# Additional compiler options (optional)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(TradingApplication PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(TradingApplication PRIVATE /W4)
endif()