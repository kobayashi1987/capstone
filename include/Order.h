//
// Created by jack on 9/10/24.
//

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>

enum class OrderType { Buy, Sell };

class Order {
public:
    Order(OrderType type, const std::string& symbol, int quantity, double price);

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

#endif // ORDER_H