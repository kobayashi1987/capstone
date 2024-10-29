//
// Created by jack on 9/10/24.
//

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>
#include "json.hpp" // Ensure nlohmann/json is included if needed for friend declarations

enum class OrderType { Buy, Sell };
enum class OrderStyle { Market, Limit };

class Order {
public:
    // Constructors
    Order();
    Order(OrderType type, OrderStyle style, const std::string& symbol, int quantity, double price,
          double stopLoss, double takeProfit,
          const std::chrono::system_clock::time_point& timestamp = std::chrono::system_clock::now());


    // Getters
    OrderType getType() const;
    OrderStyle getStyle() const;
    const std::string& getSymbol() const; // Returns a const reference
    int getQuantity() const;
    double getPrice() const;
    double getStopLoss() const;
    double getTakeProfit() const;
    std::chrono::system_clock::time_point getTimestamp() const;

    // code added on 2024.10.23
    // Equality operator
    bool operator==(const Order& other) const;

private:
    OrderType type_;
    OrderStyle style_;
    std::string symbol_;
    int quantity_;
    double price_;
    double stopLoss_;
    double takeProfit_;
    std::chrono::system_clock::time_point timestamp_;
};

// JSON serialization functions
void to_json(nlohmann::json& j, const Order& order);
void from_json(const nlohmann::json& j, Order& order);

#endif // ORDER_H