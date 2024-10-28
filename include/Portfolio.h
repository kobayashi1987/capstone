//
// Created by jack on 9/10/24.
//

// File: src/Portfolio.h

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Order.h"
#include "Trade.h"
#include <vector>
#include <unordered_map>
#include <string>

struct Position {
    int quantity;
    double averagePrice;
};

class Portfolio {
public:
    // Constructor
    Portfolio(double initialCapital);

    // Methods to place and execute orders
    void placeOrder(const Order& order, const std::unordered_map<std::string, double>& marketPrices);
    void executeTrade(const Order& order, const std::unordered_map<std::string, double>& marketPrices);

    // Methods to process orders based on market data
    void processOrders(const std::unordered_map<std::string, double>& marketPrices);
    void processStopOrders(const std::unordered_map<std::string, double>& marketPrices);
    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);

    // Getter for cash balance
    double getCashBalance() const;

    // Getter and Setter for peak value
    double getPeakValue() const;
    void setPeakValue(double peakValue);

    // Getter for drawdown
    double getDrawdown() const;

    // Getter for trade history (const)
    const std::vector<Trade>& getTradeHistory() const;

    // Getter for pending orders (const)
    const std::vector<Order>& getPendingOrders() const;

    // Getter for all orders
    std::vector<Order> getAllOrders() const;

    // Getter for positions
    const std::unordered_map<std::string, Position>& getPositions() const;


    // Setters
    void setCashBalance(double cash);
    void setDrawdown(double drawdown);

    // Add or update a position
    void addPosition(const std::string& symbol, int quantity, double price);



    // Calculate total portfolio value
    double getPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const;

    // Calculate unrealized P&L
    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;

    // Add Trade (non-const)
    void addTrade(const Trade& trade);

    // Add Pending Order (non-const)
    void addPendingOrder(const Order& order);

    // Getter for Position Value
    double getPositionValue(const std::string& symbol, const std::unordered_map<std::string, double>& marketPrices) const;

    // Add/Update Position
    void updatePosition(const std::string& symbol, int quantity, double averagePrice);

private:
    double cashBalance_;
    double peakValue_;
    double drawdown_;
    std::unordered_map<std::string, Position> positions_; // Key: Symbol, Value: Position
    std::vector<Order> pendingOrders_;
    std::vector<Trade> tradeHistory_;

    // Helper method to update peak value and drawdown
    void updatePeakValueAndDrawdown(double totalPortfolioValue);
};

#endif // PORTFOLIO_H











