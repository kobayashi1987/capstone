//
// Created by jack on 24/10/24.
//

// File: src/Utils.cpp

#include "Utils.h"
#include <ctime>
#include <string>


std::string timePointToString(const std::chrono::system_clock::time_point& tp) {
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&tt));
    return std::string(buffer);
}


