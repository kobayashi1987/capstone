////
//// Created by jack on 9/10/24.
////
//
//// File: src/TradingEngine.h
//
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




