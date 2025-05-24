//
// Created by jack on 9/10/24.
//

#include "Trade.h"

// Constructor: Initializes a Trade from an Order
Trade::Trade(const Order& order)
        : type_(order.getType()),
          symbol_(order.getSymbol()),
          quantity_(order.getQuantity()),
          price_(order.getPrice()),
          timestamp_(std::chrono::system_clock::now()),
          order_(order),                       // Initialize order_ first
          stopLoss_(order.getStopLoss()),       // Initialize stop-loss price
          takeProfit_(order.getTakeProfit())  // Initialize take-profit price

{
}

// Getters
OrderType Trade::getType() const {
    return type_;
}

// Getter for the associated Order
const Order& Trade::getOrder() const {
    return order_;
}

const std::string& Trade::getSymbol() const {
    return symbol_;
}

int Trade::getQuantity() const {
    return quantity_;
}

double Trade::getPrice() const {
    return price_;
}

std::chrono::system_clock::time_point Trade::getTimestamp() const {
    return timestamp_;
}

// New methods to access stop-loss and take-profit prices
double Trade::getStopLoss() const {
    return stopLoss_;
}

double Trade::getTakeProfit() const {
    return takeProfit_;
}

