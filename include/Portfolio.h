//
// Created by jack on 9/10/24.
//

//#ifndef PORTFOLIO_H
//#define PORTFOLIO_H
//
//#include "Order.h"
//#include "Trade.h"
//#include <string>
//#include <unordered_map>
//#include <vector>
//
//class Portfolio {
//public:
//    Portfolio(double initialCash);
//
//    // Methods for user interaction
//    void placeOrder(const Order& order);
//
//    // Updated method: 2024 Oct 9a
//    void placeOrder(const Order& order, const std::unordered_map<std::string, double>& marketPrices);
//
//    // New method to process pending orders
//    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);
//
//    const std::vector<Trade>& getTradeHistory() const;
//
//    void updatePortfolio(const std::vector<Trade>& trades);
//
//    // Getters
//    double getCashBalance() const;
//    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;
//    double getRealizedPnL() const;
//    int getPosition(const std::string& symbol) const;
//
//    // newly added : 2024 Oct 9a
//    const std::unordered_map<std::string, int>& getPositions() const;
//
//private:
//    double cashBalance_;
//    double realizedPnL_;
//    std::unordered_map<std::string, int> positions_; // symbol -> quantity
//    std::vector<Trade> tradeHistory_;
//
//    std::vector<Order> pendingOrders_; // Store pending limit orders: 2024 Oct 9a
//};
//
//#endif // PORTFOLIO_H


// Portfolio.h

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Trade.h"
#include "Order.h"
#include <string>
#include <unordered_map>
#include <vector>

class Portfolio {
public:
    Portfolio(double initialCash);

    void updatePortfolio(const std::vector<Trade>& trades);

    void placeOrder(const Order& order, const std::unordered_map<std::string, double>& marketPrices);
    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);

    // Accessor methods
    double getCashBalance() const;
    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;
    int getPosition(const std::string& symbol) const;
    const std::unordered_map<std::string, int>& getPositions() const;

private:
    double cashBalance_;
    std::unordered_map<std::string, int> positions_;
    std::vector<Trade> tradeHistory_;
    std::vector<Order> pendingOrders_; // Store pending limit orders

    // Declare executeOrder as a private method
    void executeOrder(const Order& order);

    double realizedPnL_;
};

#endif // PORTFOLIO_H