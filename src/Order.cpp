//
// Created by jack on 9/10/24.
//

#include "Order.h"

Order::Order(OrderType type, const std::string& symbol, int quantity, double price)
        : type_(type), symbol_(symbol), quantity_(quantity), price_(price), timestamp_(std::chrono::system_clock::now()) {}

OrderType Order::getType() const { return type_; }
const std::string& Order::getSymbol() const { return symbol_; }
int Order::getQuantity() const { return quantity_; }
double Order::getPrice() const { return price_; }
std::chrono::system_clock::time_point Order::getTimestamp() const { return timestamp_; }