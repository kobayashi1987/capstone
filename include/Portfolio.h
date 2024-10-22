//
// Created by jack on 9/10/24.
//



#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Order.h"
#include "Trade.h"
#include <unordered_map>
#include <vector>
#include <string>

// Structure to represent a position in a stock
struct Position {
    int quantity;           // Number of shares held (positive for long, negative for short)
    double averagePrice;    // Average price of the position
    double stopLossPrice;   // Stop-loss price for the position
    double takeProfitPrice; // Take-profit price for the position

    // Default constructor
    Position()
            : quantity(0), averagePrice(0.0), stopLossPrice(0.0), takeProfitPrice(0.0) {}


    // Constructor for Position
    Position(int qty, double avgPrice, double stopLoss = 0.0, double takeProfit = 0.0)
            : quantity(qty), averagePrice(avgPrice), stopLossPrice(stopLoss), takeProfitPrice(takeProfit) {}
};

class Portfolio {
public:
    // Constructor: Initializes the portfolio with the initial cash balance
    Portfolio(double initialCash);

    // Method to place an order
    void placeOrder(const Order& order, const std::unordered_map<std::string, double>& marketPrices);

    // Method to process pending limit orders
    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);

    // Method to process stop-loss and take-profit orders
    void processStopOrders(const std::unordered_map<std::string, double>& marketPrices);

    // Method to update the portfolio with executed trades
    void updatePortfolio(const std::vector<Trade>& executedTrades, const std::unordered_map<std::string, double>& marketPrices);

    // Method to update peak portfolio value and drawdown
    void updatePeakValueAndDrawdown(const std::unordered_map<std::string, double>& marketPrices);

    // Method to get the current cash balance
    double getCashBalance() const;

    // Method to get the total market value of all positions
    double getPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const;

    // Method to get all open positions
    const std::unordered_map<std::string, Position>& getPositions() const;

    // Method to get unrealized profit and loss across all positions
    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;

    // Method to get the market value of a specific position
    double getPositionValue(const std::string& symbol, const std::unordered_map<std::string, double>& marketPrices) const;

    // Method to get the peak portfolio value achieved
    double getPeakValue() const;

    // Method to get the current drawdown from the peak value
    double getDrawdown() const;

    // Method to set the peak portfolio value (used during initialization)
    void setPeakValue(double value);

private:
    double cashBalance_;    // Current cash balance
    std::unordered_map<std::string, Position> positions_; // Map of stock symbols to positions
    std::vector<Order> pendingOrders_;    // List of pending limit orders
    std::vector<Trade> tradeHistory_;     // History of executed trades
    double peakValue_;     // Peak portfolio value achieved
    double drawdown_;      // Current drawdown from peak value
};

#endif // PORTFOLIO_H