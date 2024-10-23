////
//// Created by jack on 9/10/24.
////
//
//
//
//#ifndef PORTFOLIO_H
//#define PORTFOLIO_H
//
//
//// JSON
//#include "json.hpp"
//using json = nlohmann::json;
//
//
//#include "Order.h"
//#include "Trade.h"
//#include <unordered_map>
//#include <vector>
//#include <string>
//
//// Structure to represent a position in a stock
//struct Position {
//    int quantity;           // Number of shares held (positive for long, negative for short)
//    double averagePrice;    // Average price of the position
//    double stopLossPrice;   // Stop-loss price for the position
//    double takeProfitPrice; // Take-profit price for the position
//
//    // Default constructor
//    Position()
//            : quantity(0), averagePrice(0.0), stopLossPrice(0.0), takeProfitPrice(0.0) {}
//
//
//    // Constructor for Position
//    Position(int qty, double avgPrice, double stopLoss = 0.0, double takeProfit = 0.0)
//            : quantity(qty), averagePrice(avgPrice), stopLossPrice(stopLoss), takeProfitPrice(takeProfit) {}
//};
//
//class Portfolio {
//public:
//    // Constructor: Initializes the portfolio with the initial cash balance
//    Portfolio(double initialCash);
//
//    // Method to place an order
//    void placeOrder(const Order& order, const std::unordered_map<std::string, double>& marketPrices);
//
//    // Method to process pending limit orders
//    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);
//
//    // Method to process stop-loss and take-profit orders
//    void processStopOrders(const std::unordered_map<std::string, double>& marketPrices);
//
//    // Method to update the portfolio with executed trades
//    void updatePortfolio(const std::vector<Trade>& executedTrades, const std::unordered_map<std::string, double>& marketPrices);
//
//    // Method to update peak portfolio value and drawdown
//    void updatePeakValueAndDrawdown(const std::unordered_map<std::string, double>& marketPrices);
//
//    // Method to get the current cash balance
//    double getCashBalance() const;
//
//    // Method to get the total market value of all positions
//    double getPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const;
//
//    // Method to get all open positions
//    const std::unordered_map<std::string, Position>& getPositions() const;
//
//    // Method to get unrealized profit and loss across all positions
//    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;
//
//    // Method to get the market value of a specific position
//    double getPositionValue(const std::string& symbol, const std::unordered_map<std::string, double>& marketPrices) const;
//
//    // Method to get the peak portfolio value achieved
//    double getPeakValue() const;
//
//    // Method to get the current drawdown from the peak value
//    double getDrawdown() const;
//
//    // Method to set the peak portfolio value (used during initialization)
//    void setPeakValue(double value);
//
//
//
//    // code added 2024.10.23
//
//private:
//    double cashBalance_;    // Current cash balance
//    std::unordered_map<std::string, Position> positions_; // Map of stock symbols to positions
//    std::vector<Order> pendingOrders_;    // List of pending limit orders
//    std::vector<Trade> tradeHistory_;     // History of executed trades
//    double peakValue_;     // Peak portfolio value achieved
//    double drawdown_;      // Current drawdown from peak value
//};
//
//#endif // PORTFOLIO_H

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











