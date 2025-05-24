//
// Created by jack on 10/10/24.
//

#include "MomentumStrategy.h"
#include "Order.h"
#include <stdexcept>

MomentumStrategy::MomentumStrategy(int lookbackPeriod, double buyThreshold, double sellThreshold)
        : lookbackPeriod_(lookbackPeriod), buyThreshold_(buyThreshold), sellThreshold_(sellThreshold) {
    if (lookbackPeriod_ <= 0) {
        throw std::invalid_argument("Look-back period must be a positive integer");
    }
    if (buyThreshold_ <= sellThreshold_) {
        throw std::invalid_argument("Buy threshold must be greater than sell threshold");
    }
}

void MomentumStrategy::generateSignals(const std::vector<double>& prices) {
    if (prices.size() < static_cast<size_t>(lookbackPeriod_ + 1)) {
        throw std::invalid_argument("Not enough price data to calculate momentum");
    }

    signals_.resize(prices.size(), 0);
    momentumValues_.resize(prices.size(), 0.0);

    for (size_t i = lookbackPeriod_; i < prices.size(); ++i) {
        double momentum = (prices[i] - prices[i - lookbackPeriod_]) / prices[i - lookbackPeriod_];
        momentumValues_[i] = momentum;

        if (momentum >= buyThreshold_) {
            signals_[i] = 1; // Buy signal
        } else if (momentum <= sellThreshold_) {
            signals_[i] = -1; // Sell signal
        } else {
            signals_[i] = 0; // Neutral
        }
    }
}

std::vector<Order> MomentumStrategy::generateOrders(const std::vector<double>& prices, const std::string& symbol) {
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

const std::vector<double>& MomentumStrategy::getMomentumValues() const {
    return momentumValues_;
}

const std::vector<int>& MomentumStrategy::getSignals() const {
    return signals_;
}