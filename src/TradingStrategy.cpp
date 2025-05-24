//
// Created by jack on 7/10/24.
//

// src/TradingStrategy.cpp

#include "TradingStrategy.h"


const std::vector<int>& TradingStrategy::getSignals() const {
    return signals_;
}

// Newly added method to generate orders
std::vector<Order> TradingStrategy::generateOrders(const std::vector<double>& prices, const std::string& symbol) {
    // Default implementation (empty)
    return {};
}