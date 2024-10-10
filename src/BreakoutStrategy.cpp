//
// Created by jack on 10/10/24.
//


#include "BreakoutStrategy.h"
#include "Order.h"
#include <stdexcept>
#include <algorithm>

BreakoutStrategy::BreakoutStrategy(int lookbackPeriod)
        : lookbackPeriod_(lookbackPeriod) {
    if (lookbackPeriod_ <= 0) {
        throw std::invalid_argument("Look-back period must be a positive integer");
    }
}

void BreakoutStrategy::generateSignals(const std::vector<double>& prices) {
    if (prices.size() < static_cast<size_t>(lookbackPeriod_)) {
        throw std::invalid_argument("Not enough price data to calculate breakout levels");
    }

    size_t dataSize = prices.size();
    signals_.resize(dataSize, 0);
    resistanceLevels_.resize(dataSize, 0.0);
    supportLevels_.resize(dataSize, 0.0);

    for (size_t i = lookbackPeriod_; i < dataSize; ++i) {
        auto startIt = prices.begin() + i - lookbackPeriod_;
        auto endIt = prices.begin() + i;

        double highestHigh = *std::max_element(startIt, endIt);
        double lowestLow = *std::min_element(startIt, endIt);

        resistanceLevels_[i] = highestHigh;
        supportLevels_[i] = lowestLow;

        double currentPrice = prices[i];

        if (currentPrice > highestHigh) {
            signals_[i] = 1; // Buy signal
        } else if (currentPrice < lowestLow) {
            signals_[i] = -1; // Sell signal
        } else {
            signals_[i] = 0; // Neutral
        }
    }
}

std::vector<Order> BreakoutStrategy::generateOrders(const std::vector<double>& prices, const std::string& symbol) {
    generateSignals(prices);

    std::vector<Order> orders;

    for (size_t i = lookbackPeriod_; i < signals_.size(); ++i) {
        if (signals_[i] == 1) {
            // Buy signal
            orders.emplace_back(OrderType::Buy, OrderStyle::Market, symbol, /*quantity=*/100, prices[i]);
        } else if (signals_[i] == -1) {
            // Sell signal
            orders.emplace_back(OrderType::Sell, OrderStyle::Market, symbol, /*quantity=*/100, prices[i]);
        }
    }

    return orders;
}

const std::vector<int>& BreakoutStrategy::getSignals() const {
    return signals_;
}

const std::vector<double>& BreakoutStrategy::getResistanceLevels() const {
    return resistanceLevels_;
}

const std::vector<double>& BreakoutStrategy::getSupportLevels() const {
    return supportLevels_;
}