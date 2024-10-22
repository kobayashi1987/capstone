//
// Created by jack on 9/10/24.
//

//#ifndef TRADINGENGINE_H
//#define TRADINGENGINE_H
//
//#include "Portfolio.h"
//#include "TradingStrategy.h"
//#include "OrderExecutor.h"
//#include "MarketDataFeed.h" // Include this
//
//#include <memory>
//#include <string>
//#include <unordered_map>
//
//
//class TradingEngine {
//public:
//    TradingEngine(double initialCash);
//
//    // Set and execute strategy
//    void setStrategy(std::unique_ptr<TradingStrategy> strategy);
//    void executeStrategy(const std::vector<double>& prices, const std::string& symbol);
//
//    // User interaction methods
//    void userBuy(const std::string& symbol, int quantity, double price);
//    void userSell(const std::string& symbol, int quantity, double price);
//
//    // Updated user interaction methods: 2024 Oct 9a
//    void userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style, int quantity, double price, const std::unordered_map<std::string, double>& marketPrices);
//
//    // New method to process pending orders
//    void processPendingOrders();
//    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);
//
//    // Portfolio access
//    double getCashBalance() const;
//    int getPosition(const std::string& symbol) const;
//    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;
//
//
//
//    // code added on 2024.10.22
//    // New method to calculate position size based on risk per trade
//    int calculatePositionSize(double entryPrice, double stopLossPrice, double riskPerTrade);
//
//    // Updated method to place orders with risk management
//    void userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style,
//                        double entryPrice, double stopLossPrice, double takeProfitPrice,
//                        const std::unordered_map<std::string, double>& marketPrices);
//    // code added on 2024.10.22
//
//
//
//    // newly added : 2024 Oct 9a
//    const std::unordered_map<std::string, int>& getPositions() const;
////    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);
//
//private:
//    std::unique_ptr<TradingStrategy> strategy_;
//    OrderExecutor executor_;
//    Portfolio portfolio_;
//
//    MarketDataFeed marketDataFeed_; // Declare marketDataFeed_ as a member variable
//
//
//};
//
//#endif // TRADINGENGINE_H


// include/TradingEngine.h

#ifndef TRADINGENGINE_H
#define TRADINGENGINE_H

#include "Portfolio.h"
#include "Order.h"
#include <unordered_map>
#include <string>

// The TradingEngine class coordinates user inputs, market data, and portfolio management.
// It handles order placement, risk management, and updates to the portfolio based on market data.

class TradingEngine {
public:
    // Constructor: Initializes the TradingEngine with the initial capital
    TradingEngine(double initialCapital);

    // Method to place an order with risk management features
    // This method includes:
    // - Position sizing based on risk per trade
    // - Risk-reward ratio checks
    // - Maximum drawdown limits
    // - Maximum allocation per asset
    void userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style,
                        double entryPrice, double stopLossPrice, double takeProfitPrice,
                        const std::unordered_map<std::string, double>& marketPrices);

    // Method to calculate position size based on risk per trade
    // Calculates the number of shares to trade to limit risk to a specified amount
    int calculatePositionSize(double entryPrice, double stopLossPrice, double riskPerTrade);

    // Method to update market data and process stop-loss and take-profit orders
    // This method should be called whenever market prices are updated
    void updateMarketData(const std::unordered_map<std::string, double>& marketPrices);

    // Method to get the current cash balance
    double getCashBalance() const;

    // Method to get all open positions
    const std::unordered_map<std::string, Position>& getPositions() const;

    // Method to get the unrealized profit and loss across all positions
    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;

    // Method to get the total portfolio value (cash balance + market value of positions)
    double getTotalPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const;

private:
    Portfolio portfolio_; // Manages the user's holdings, cash balance, and risk management

    // Additional private methods or members can be added here if necessary
};

#endif // TRADINGENGINE_H