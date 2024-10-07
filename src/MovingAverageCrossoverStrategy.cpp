//
// Created by jack on 7/10/24.
//

// src/MovingAverageCrossoverStrategy.cpp

#include "MovingAverageCrossoverStrategy.h"
#include <numeric>   // For std::accumulate
#include <stdexcept> // For std::invalid_argument

MovingAverageCrossoverStrategy::MovingAverageCrossoverStrategy(int shortWindow, int longWindow)
        : shortWindow_(shortWindow), longWindow_(longWindow) {
    if (shortWindow <= 0 || longWindow <= 0) {
        throw std::invalid_argument("Window sizes must be positive integers.");
    }
    if (shortWindow >= longWindow) {
        throw std::invalid_argument("Short window must be smaller than long window.");
    }
}

double MovingAverageCrossoverStrategy::calculateMovingAverage(const std::vector<double>& prices, size_t startIndex, int windowSize) {
    if (startIndex + windowSize > prices.size()) {
        return 0.0;
    }
    double sum = std::accumulate(prices.begin() + startIndex, prices.begin() + startIndex + windowSize, 0.0);
    return sum / windowSize;
}

void MovingAverageCrossoverStrategy::generateSignals(const std::vector<double>& prices) {
    signals_.clear();
    signals_.resize(prices.size(), 0);

    if (prices.size() < static_cast<size_t>(longWindow_)) {
        // Not enough data to generate signals
        return;
    }

    for (size_t i = 0; i <= prices.size() - longWindow_; ++i) {
        double shortMA = calculateMovingAverage(prices, i + longWindow_ - shortWindow_, shortWindow_);
        double longMA = calculateMovingAverage(prices, i, longWindow_);

        if (shortMA > longMA) {
            signals_[i + longWindow_ - 1] = 1; // Buy signal
        } else if (shortMA < longMA) {
            signals_[i + longWindow_ - 1] = -1; // Sell signal
        } else {
            signals_[i + longWindow_ - 1] = 0; // Hold/Neutral
        }
    }
}