//
// Created by jack on 9/10/24.
//


// code added on 2024.10.22
//#include "Order.h"
//
//Order::Order(OrderType type, OrderStyle style, const std::string& symbol, int quantity, double price,
//             double stopLoss, double takeProfit)
//        : type_(type), style_(style), symbol_(symbol), quantity_(quantity), price_(price),
//          stopLoss_(stopLoss), takeProfit_(takeProfit), timestamp_(std::chrono::system_clock::now()) {}
//
//OrderType Order::getType() const { return type_; }
//OrderStyle Order::getStyle() const { return style_; }
//const std::string& Order::getSymbol() const { return symbol_; }
//int Order::getQuantity() const { return quantity_; }
//double Order::getPrice() const { return price_; }
//double Order::getStopLoss() const { return stopLoss_; }
//double Order::getTakeProfit() const { return takeProfit_; }
//std::chrono::system_clock::time_point Order::getTimestamp() const { return timestamp_; }
// code added on 2024.10.22


// File: src/Order.cpp

//
// Created by jack on 9/10/24.
//
#include "Utils.h"           // Include the Utils header for timePointToString
#include "Order.h"           // Include the Order class header
#include "json.hpp"          // Include nlohmann/json for JSON operations
#include <stdexcept>        // For exception handling
#include <sstream>          // For string stream operations
#include <iomanip>          // For input/output manipulators

using json = nlohmann::json;

// ================== Helper Functions ==================

// Convert string in "YYYY-MM-DD HH:MM:SS" format to time_point
std::chrono::system_clock::time_point stringToTimePoint(const std::string& s) {
    std::tm tm = {};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::invalid_argument("Failed to parse time string: " + s);
    }
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}


// ================== Order Class Member Function Implementations ==================

// Default Constructor
Order::Order()
        : type_(OrderType::Buy),
          style_(OrderStyle::Market),
          symbol_(""),
          quantity_(0),
          price_(0.0),
          stopLoss_(0.0),
          takeProfit_(0.0),
          timestamp_(std::chrono::system_clock::now()) {}




// Parameterized Constructor
Order::Order(OrderType type, OrderStyle style, const std::string& symbol, int quantity, double price,
             double stopLoss, double takeProfit,
             const std::chrono::system_clock::time_point& timestamp)
        : type_(type),
          style_(style),
          symbol_(symbol),
          quantity_(quantity),
          price_(price),
          stopLoss_(stopLoss),
          takeProfit_(takeProfit),
          timestamp_(timestamp) {}

// Getter for Order Type
OrderType Order::getType() const {
    return type_;
}

// Getter for Order Style
OrderStyle Order::getStyle() const {
    return style_;
}

// Getter for Symbol
const std::string& Order::getSymbol() const { // Must match the declaration in Order.h
    return symbol_;
}

// Getter for Quantity
int Order::getQuantity() const {
    return quantity_;
}

// Getter for Price
double Order::getPrice() const {
    return price_;
}

// Getter for Stop-Loss Price
double Order::getStopLoss() const {
    return stopLoss_;
}

// Getter for Take-Profit Price
double Order::getTakeProfit() const {
    return takeProfit_;
}

// Getter for Timestamp
std::chrono::system_clock::time_point Order::getTimestamp() const {
    return timestamp_;
}

// ================== Equality Operator Implementation ==================

// Equality operator
bool Order::operator==(const Order& other) const {
    return (type_ == other.type_) &&
           (style_ == other.style_) &&
           (symbol_ == other.symbol_) &&
           (quantity_ == other.quantity_) &&
           (price_ == other.price_) &&
           (stopLoss_ == other.stopLoss_) &&
           (takeProfit_ == other.takeProfit_) &&
           (timestamp_ == other.timestamp_);
}

// ================== JSON Serialization and Deserialization ==================

// to_json function for Order class
void to_json(json& j, const Order& order) {
    j = json{
            {"type", order.getType() == OrderType::Buy ? "Buy" : "Sell"},
            {"style", order.getStyle() == OrderStyle::Market ? "Market" : "Limit"},
            {"symbol", order.getSymbol()},
            {"quantity", order.getQuantity()},
            {"price", order.getPrice()},
            {"stopLossPrice", order.getStopLoss()},
            {"takeProfitPrice", order.getTakeProfit()},
            {"timestamp", timePointToString(order.getTimestamp())}
    };
}

// from_json function for Order class
void from_json(const json& j, Order& order) {
    std::string typeStr, styleStr, symbol, timestampStr;
    int quantity;
    double price, stopLoss, takeProfit;

    // Extract fields from JSON
    j.at("type").get_to(typeStr);
    j.at("style").get_to(styleStr);
    j.at("symbol").get_to(symbol);
    j.at("quantity").get_to(quantity);
    j.at("price").get_to(price);
    j.at("stopLossPrice").get_to(stopLoss);
    j.at("takeProfitPrice").get_to(takeProfit);
    j.at("timestamp").get_to(timestampStr);

    // Convert type string to OrderType enum
    OrderType type;
    if (typeStr == "Buy") {
        type = OrderType::Buy;
    }
    else if (typeStr == "Sell") {
        type = OrderType::Sell;
    }
    else {
        throw std::invalid_argument("Invalid OrderType: " + typeStr);
    }

    // Convert style string to OrderStyle enum
    OrderStyle style;
    if (styleStr == "Market") {
        style = OrderStyle::Market;
    }
    else if (styleStr == "Limit") {
        style = OrderStyle::Limit;
    }
    else {
        throw std::invalid_argument("Invalid OrderStyle: " + styleStr);
    }

    // Convert timestamp string to time_point
    std::chrono::system_clock::time_point timestamp = stringToTimePoint(timestampStr);

    // Assign the parsed values to the Order object
    order = Order(type, style, symbol, quantity, price, stopLoss, takeProfit, timestamp);
}

