//
// Created by jack on 9/10/24.
//

#ifndef TRADE_H
#define TRADE_H

#include "Order.h"

class Trade {
public:
    Trade(const Order& order);

    // Getters
    OrderType getType() const;
    const std::string& getSymbol() const;
    int getQuantity() const;
    double getPrice() const;
    std::chrono::system_clock::time_point getTimestamp() const;

private:
    OrderType type_;
    std::string symbol_;
    int quantity_;
    double price_;
    std::chrono::system_clock::time_point timestamp_;
};

#endif // TRADE_H