//
// Created by jack on 7/10/24.
//
#include "RSIStrategy.h"
#include <stdexcept>

RSIStrategy::RSIStrategy(int period, double overbought, double oversold)
        : period_(period), overbought_(overbought), oversold_(oversold) {
    if (period <= 0) {
        throw std::invalid_argument("Period must be positive.");
    }
}

std::vector<double> RSIStrategy::calculateRSI(const std::vector<double>& prices) {
    std::vector<double> rsi(prices.size(), 0.0);

    if (prices.size() <= static_cast<size_t>(period_)) {
        return rsi;
    }

    std::vector<double> gains, losses;

    for (size_t i = 1; i < prices.size(); ++i) {
        double change = prices[i] - prices[i - 1];
        if (change >= 0) {
            gains.push_back(change);
            losses.push_back(0.0);
        } else {
            gains.push_back(0.0);
            losses.push_back(-change);
        }
    }

    double avgGain = 0.0, avgLoss = 0.0;
    for (size_t i = 0; i < static_cast<size_t>(period_); ++i) {
        avgGain += gains[i];
        avgLoss += losses[i];
    }
    avgGain /= period_;
    avgLoss /= period_;

    rsi[period_] = 100.0 - (100.0 / (1.0 + (avgGain / avgLoss)));

    for (size_t i = period_ + 1; i < gains.size(); ++i) {
        avgGain = ((avgGain * (period_ - 1)) + gains[i]) / period_;
        avgLoss = ((avgLoss * (period_ - 1)) + losses[i]) / period_;
        rsi[i + 1] = 100.0 - (100.0 / (1.0 + (avgGain / avgLoss)));
    }

    return rsi;
}

void RSIStrategy::generateSignals(const std::vector<double>& prices) {
    signals_.clear();
    signals_.resize(prices.size(), 0);

    auto rsiValues = calculateRSI(prices);

    for (size_t i = 0; i < rsiValues.size(); ++i) {
        if (rsiValues[i] > overbought_) {
            signals_[i] = -1; // Sell signal
        } else if (rsiValues[i] < oversold_) {
            signals_[i] = 1;  // Buy signal
        } else {
            signals_[i] = 0;  // Hold
        }
    }
}