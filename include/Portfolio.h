////
//// Created by jack on 9/10/24.
////
//
//// File: src/Portfolio.h
//
#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Order.h"
#include "Trade.h"
#include "ProfitLoss.h"  // newly added
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

    // Method to execute a trade
    void executeTrade(const Order& order, const std::unordered_map<std::string, double>& marketPrices);

    // Methods to process orders based on market data
    void processOrders(const std::unordered_map<std::string, double>& marketPrices);
    void processStopOrders(const std::unordered_map<std::string, double>& marketPrices);
    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);

    // Getters for peak value, cash balance and drawdown
    double getCashBalance() const;
    double getPeakValue() const;
    double getDrawdown() const;

    // Getter for trade history (const), pending orders (const), and positions (const), and all orders
    const std::vector<Trade>& getTradeHistory() const;
    const std::vector<Order>& getPendingOrders() const;
    std::vector<Order> getAllOrders() const;
    const std::unordered_map<std::string, Position>& getPositions() const;


    //Portfolio metrics:  Calculate total portfolio value + Calculate unrealized P&L
    double getPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const;
    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;


    // P&L List
    const std::vector<ProfitLoss>& getProfitLossList() const; // Newly added

    // Methods to manage P&L
    void addProfitLoss(const ProfitLoss& pl); // Newly added


    // Additional methods
    void addPendingOrder(const Order& order); // Add Pending Order (non-const)
    void addPosition(const std::string& symbol, int quantity, double price); // Add or update a position
    void addTrade(const Trade& trade); // Add Trade (non-const)
    void updatePosition(const std::string& symbol, int quantity, double averagePrice); // Add/Update Position
    // Getter for Position Value
    double getPositionValue(const std::string& symbol, const std::unordered_map<std::string, double>& marketPrices) const;

    // Setters
    void setCashBalance(double cash);
    void setDrawdown(double drawdown);
    void setPeakValue(double peakValue);


    // Method to set P&L list (for loading from persistence)
    void setProfitLossList(const std::vector<ProfitLoss>& plList); // Newly added


private:
    double cashBalance_;
    double peakValue_;
    double drawdown_;

    std::unordered_map<std::string, Position> positions_; // Key: Symbol, Value: Position
    std::vector<Order> pendingOrders_;
    std::vector<Trade> tradeHistory_;

    // P&L list
    std::vector<ProfitLoss> plList_; // Newly added

    // Helper method to update peak value and drawdown
    void updatePeakValueAndDrawdown(double totalPortfolioValue);
};

#endif // PORTFOLIO_H


