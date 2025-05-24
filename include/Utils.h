//
// Created by jack on 24/10/24.
//

// File: src/Utils.h

#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>


std::string timePointToString(const std::chrono::system_clock::time_point& tp);

#endif // UTILS_H