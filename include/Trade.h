//
// Created by jack on 9/10/24.
//

#ifndef TRADE_H
#define TRADE_H

#include "Order.h"
#include <chrono>

class Trade {
public:
    Trade(const Order& order);

    // Getters
    OrderType getType() const;
    const std::string& getSymbol() const;
    int getQuantity() const;
    double getPrice() const;
    std::chrono::system_clock::time_point getTimestamp() const;

    // Getter for the associated Order
    const Order& getOrder() const;


    // New methods to access stop-loss and take-profit prices
    double getStopLoss() const;
    double getTakeProfit() const;

private:
    OrderType type_;
    std::string symbol_;
    int quantity_;
    double price_;
    std::chrono::system_clock::time_point timestamp_;
    Order order_;  // Add order as a member variable

    // Add stop-loss and take-profit prices as member variables
    double stopLoss_;
    double takeProfit_;
};

#endif // TRADE_H