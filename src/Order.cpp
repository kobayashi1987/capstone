//
// Created by jack on 9/10/24.
//

//#include "Order.h"

//Order::Order(OrderType type, OrderStyle style, const std::string& symbol, int quantity, double price)
//        : type_(type), style_(style), symbol_(symbol), quantity_(quantity), price_(price), timestamp_(std::chrono::system_clock::now()) {}

// code added on 2024.10.22
//Order::Order(OrderType type, OrderStyle style, const std::string& symbol, int quantity, double price,
//             double stopLoss, double takeProfit)
//        : type_(type), style_(style), symbol_(symbol), quantity_(quantity), price_(price),
//          stopLoss_(stopLoss), takeProfit_(takeProfit), timestamp_(std::chrono::system_clock::now())
// code added on 2024.10.22


//OrderType Order::getType() const { return type_; }

//OrderStyle Order::getStyle() const { return style_; }
//const std::string& Order::getSymbol() const { return symbol_; }
//int Order::getQuantity() const { return quantity_; }
//double Order::getPrice() const { return price_; }
//std::chrono::system_clock::time_point Order::getTimestamp() const { return timestamp_; }

// New getters for stop-loss and take-profit: 2024 Oct 21
// Getters
//double Order::getStopLoss() const {
//    return stopLoss_;
//}
//
//double Order::getTakeProfit() const {
//    return takeProfit_;
//}

// code added on 2024.10.22
#include "Order.h"

Order::Order(OrderType type, OrderStyle style, const std::string& symbol, int quantity, double price,
             double stopLoss, double takeProfit)
        : type_(type), style_(style), symbol_(symbol), quantity_(quantity), price_(price),
          stopLoss_(stopLoss), takeProfit_(takeProfit), timestamp_(std::chrono::system_clock::now()) {}

OrderType Order::getType() const { return type_; }
OrderStyle Order::getStyle() const { return style_; }
const std::string& Order::getSymbol() const { return symbol_; }
int Order::getQuantity() const { return quantity_; }
double Order::getPrice() const { return price_; }
double Order::getStopLoss() const { return stopLoss_; }
double Order::getTakeProfit() const { return takeProfit_; }
std::chrono::system_clock::time_point Order::getTimestamp() const { return timestamp_; }
// code added on 2024.10.22