
// File: src/TradingEngine.cpp

#include "TradingEngine.h"
#include "Portfolio.h"
#include <iostream>

// Constructor
TradingEngine::TradingEngine(double initialCapital)
        : portfolio_(initialCapital) {}

// Set Portfolio (used when loading from persistence)
void TradingEngine::setPortfolio(const Portfolio& portfolio) {
    portfolio_ = portfolio;
}

// Place an order (existing function)
void TradingEngine::placeOrder(const std::string& symbol, OrderType type, OrderStyle style, double price,
                               double stopLossPrice, double takeProfitPrice,
                               const std::unordered_map<std::string, double>& marketPrices) {
    Order order(type, style, symbol, 100, price, stopLossPrice, takeProfitPrice);
    portfolio_.executeTrade(order, marketPrices);
}

// User Place Order (modified function to accept quantity)
void TradingEngine::userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style, int quantity, double price,
                                   double stopLossPrice, double takeProfitPrice,
                                   const std::unordered_map<std::string, double>& marketPrices) {
    // Create the Order object with specified quantity
    Order order(type, style, symbol, quantity, price, stopLossPrice, takeProfitPrice);

    if (style == OrderStyle::Market) {
        // Execute Market Order Immediately
        try {
            portfolio_.executeTrade(order, marketPrices);
            std::cout << "Market Order executed successfully.\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error executing Market Order: " << e.what() << "\n";
        }
    }
    else if (style == OrderStyle::Limit) {
        // Add Limit Order to Pending Orders
        portfolio_.addPendingOrder(order);
        std::cout << "Limit Order added to pending orders.\n";
    }
}
// Update market data and process orders
void TradingEngine::updateMarketData(const std::unordered_map<std::string, double>& marketPrices) {
    // Process Pending Orders
    portfolio_.processOrders(marketPrices);
}

// Getters
Portfolio& TradingEngine::getPortfolio() { return portfolio_; }
const Portfolio& TradingEngine::getPortfolio() const { return portfolio_; }

std::vector<Order> TradingEngine::getAllOrders() const { return portfolio_.getPendingOrders(); }

double TradingEngine::getCashBalance() const {
    return portfolio_.getCashBalance();
}

const std::unordered_map<std::string, Position>& TradingEngine::getPositions() const {
    return portfolio_.getPositions();
}

// <-- Corrected Function Name
double TradingEngine::getTotalPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const {
    return portfolio_.getPortfolioValue(marketPrices); // Updated to match Portfolio.h
}