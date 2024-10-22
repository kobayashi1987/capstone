//
// Created by jack on 9/10/24.
//

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>

enum class OrderType { Buy, Sell };
enum class OrderStyle { Market, Limit };

class Order {
public:

    // Order(OrderType type, OrderStyle style, const std::string& symbol, int quantity, double price);
    // Updated constructor with stop-loss and take-profit parameters
    // Code added on 2024.10.22
    Order(OrderType type, OrderStyle style, const std::string& symbol, int quantity, double price,
          double stopLoss = 0.0, double takeProfit = 0.0);
    // Code added on 2024.10.22

    // Getters
    OrderType getType() const;
    OrderStyle getStyle() const;
    const std::string& getSymbol() const;
    int getQuantity() const;
    double getPrice() const;
    std::chrono::system_clock::time_point getTimestamp() const;

    // New getters for stop-loss and take-profit
    // Code added on 2024.10.22
    double getStopLoss() const;
    double getTakeProfit() const;
    // Code added on 2024.10.22

private:
    OrderType type_;
    OrderStyle style_;
    std::string symbol_;
    int quantity_;
    double price_;
    std::chrono::system_clock::time_point timestamp_;

    // Code added on 2024.10.22
    double stopLoss_;
    double takeProfit_;
    // Code added on 2024.10.22
};

#endif // ORDER_H