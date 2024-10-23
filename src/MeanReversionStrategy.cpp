//
// Created by jack on 20/10/24.
//


#include "MeanReversionStrategy.h"
#include "Order.h"
#include <stdexcept>
#include <numeric>
#include <cmath>

MeanReversionStrategy::MeanReversionStrategy(int lookbackPeriod, double deviationThreshold)
        : lookbackPeriod_(lookbackPeriod), deviationThreshold_(deviationThreshold) {
    if (lookbackPeriod_ <= 0) {
        throw std::invalid_argument("Look-back period must be a positive integer");
    }
    if (deviationThreshold_ <= 0) {
        throw std::invalid_argument("Deviation threshold must be a positive number");
    }
}

void MeanReversionStrategy::generateSignals(const std::vector<double>& prices) {
    if (prices.size() < static_cast<size_t>(lookbackPeriod_)) {
        throw std::invalid_argument("Not enough price data to calculate mean");
    }

    size_t dataSize = prices.size();
    signals_.resize(dataSize, 0);
    meanValues_.resize(dataSize, 0.0);

    for (size_t i = lookbackPeriod_; i < dataSize; ++i) {
        double sum = std::accumulate(prices.begin() + i - lookbackPeriod_, prices.begin() + i, 0.0);
        double mean = sum / lookbackPeriod_;
        meanValues_[i] = mean;

        double deviation = (prices[i] - mean) / mean;

        if (deviation <= -deviationThreshold_) {
            signals_[i] = 1; // Buy signal
        } else if (deviation >= deviationThreshold_) {
            signals_[i] = -1; // Sell signal
        } else {
            signals_[i] = 0; // Neutral
        }
    }
}

std::vector<Order> MeanReversionStrategy::generateOrders(const std::vector<double>& prices, const std::string& symbol) {
    generateSignals(prices);

    std::vector<Order> orders;

    for (size_t i = lookbackPeriod_; i < signals_.size(); ++i) {
        if (signals_[i] == 1) {
            // Buy signal
            orders.emplace_back(Order(OrderType::Buy, OrderStyle::Market, symbol, 100, prices[i], 0.0, 0.0));
        } else if (signals_[i] == -1) {
            // Sell signal
            orders.emplace_back(Order(OrderType::Sell, OrderStyle::Market, symbol, 100, prices[i], 0.0, 0.0));
        }
    }

    return orders;
}

const std::vector<int>& MeanReversionStrategy::getSignals() const {
    return signals_;
}

const std::vector<double>& MeanReversionStrategy::getMeanValues() const {
    return meanValues_;
}