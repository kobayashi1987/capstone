////
//// Created by jack on 9/10/24.
////
//
//
//#ifndef TRADINGENGINE_H
//#define TRADINGENGINE_H
//
//#include "Portfolio.h"
//#include "Order.h"
//#include <unordered_map>
//#include <string>
//
//// The TradingEngine class coordinates user inputs, market data, and portfolio management.
//// It handles order placement, risk management, and updates to the portfolio based on market data.
//
//class TradingEngine {
//public:
//    // Constructor: Initializes the TradingEngine with the initial capital
//    TradingEngine(double initialCapital);
//
//    // Method to place an order with risk management features
//    // This method includes:
//    // - Position sizing based on risk per trade
//    // - Risk-reward ratio checks
//    // - Maximum drawdown limits
//    // - Maximum allocation per asset
//    void userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style,
//                        double entryPrice, double stopLossPrice, double takeProfitPrice,
//                        const std::unordered_map<std::string, double>& marketPrices);
//
//    // Method to calculate position size based on risk per trade
//    // Calculates the number of shares to trade to limit risk to a specified amount
//    int calculatePositionSize(double entryPrice, double stopLossPrice, double riskPerTrade);
//
//    // Method to update market data and process stop-loss and take-profit orders
//    // This method should be called whenever market prices are updated
//    void updateMarketData(const std::unordered_map<std::string, double>& marketPrices);
//
//    // Method to get the current cash balance
//    double getCashBalance() const;
//
//    // Method to get all open positions
//    const std::unordered_map<std::string, Position>& getPositions() const;
//
//    // Method to get the unrealized profit and loss across all positions
//    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;
//
//    // Method to get the total portfolio value (cash balance + market value of positions)
//    double getTotalPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const;
//
//private:
//    Portfolio portfolio_; // Manages the user's holdings, cash balance, and risk management
//
//    // Additional private methods or members can be added here if necessary
//};
//
//#endif // TRADINGENGINE_H

// File: src/TradingEngine.h

#ifndef TRADINGENGINE_H
#define TRADINGENGINE_H

#include "Portfolio.h"
#include "Order.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <exception>

class TradingEngine {
public:
    // Constructor
    TradingEngine(double initialCapital);

    // Set Portfolio (used when loading from persistence)
    void setPortfolio(const Portfolio& portfolio);

    // Place an order (existing function)
    void placeOrder(const std::string& symbol, OrderType type, OrderStyle style, double price,
                    double stopLossPrice, double takeProfitPrice,
                    const std::unordered_map<std::string, double>& marketPrices);

    // User Place Order (new function)
    void userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style, double price,
                        double stopLossPrice, double takeProfitPrice,
                        const std::unordered_map<std::string, double>& marketPrices);

    // Update market data and process orders
    void updateMarketData(const std::unordered_map<std::string, double>& marketPrices);

    // Getters
    Portfolio& getPortfolio();
    const Portfolio& getPortfolio() const;

    // Get all orders
    std::vector<Order> getAllOrders() const;

    // New Getters
    double getCashBalance() const;
    const std::unordered_map<std::string, Position>& getPositions() const;
    double getTotalPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const;

private:
    Portfolio portfolio_;
};

#endif // TRADINGENGINE_H