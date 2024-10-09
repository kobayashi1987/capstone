//
// Created by jack on 9/10/24.
//

#ifndef TRADINGENGINE_H
#define TRADINGENGINE_H

#include "Portfolio.h"
#include "TradingStrategy.h"
#include "OrderExecutor.h"
#include "MarketDataFeed.h" // Include this

#include <memory>
#include <string>
#include <unordered_map>


class TradingEngine {
public:
    TradingEngine(double initialCash);

    // Set and execute strategy
    void setStrategy(std::unique_ptr<TradingStrategy> strategy);
    void executeStrategy(const std::vector<double>& prices, const std::string& symbol);

    // User interaction methods
    void userBuy(const std::string& symbol, int quantity, double price);
    void userSell(const std::string& symbol, int quantity, double price);

    // Updated user interaction methods: 2024 Oct 9a
    void userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style, int quantity, double price, const std::unordered_map<std::string, double>& marketPrices);

    // New method to process pending orders
    void processPendingOrders();
    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);

    // Portfolio access
    double getCashBalance() const;
    int getPosition(const std::string& symbol) const;
    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;



    // newly added : 2024 Oct 9a
    const std::unordered_map<std::string, int>& getPositions() const;
//    void processPendingOrders(const std::unordered_map<std::string, double>& marketPrices);

private:
    std::unique_ptr<TradingStrategy> strategy_;
    OrderExecutor executor_;
    Portfolio portfolio_;

    MarketDataFeed marketDataFeed_; // Declare marketDataFeed_ as a member variable


};

#endif // TRADINGENGINE_H