//
// Created by jack on 9/10/24.
//

#include "TradingEngine.h"
#include <iostream>
#include <fstream>
#include "json.hpp"// Include JSON library for P&L

using json = nlohmann::json;

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
    Order order(type, style, symbol, 10, price, stopLossPrice, takeProfitPrice); // Assuming fixed quantity
    portfolio_.executeTrade(order, marketPrices);
}


// Update market data and process orders
void TradingEngine::updateMarketData(const std::unordered_map<std::string, double>& marketPrices) {
    portfolio_.processOrders(marketPrices);
}

// Getter for Portfolio (modifiable)
Portfolio& TradingEngine::getPortfolio() {
    return portfolio_;
}

// Getter for Portfolio (read-only)
const Portfolio& TradingEngine::getPortfolio() const {
    return portfolio_;
}

// Get all orders
std::vector<Order> TradingEngine::getAllOrders() const {
    return portfolio_.getAllOrders();
}

// New Getter: getCashBalance
double TradingEngine::getCashBalance() const {
    return portfolio_.getCashBalance();
}

// New Getter: getPositions
const std::unordered_map<std::string, Position>& TradingEngine::getPositions() const {
    return portfolio_.getPositions();
}

// New Getter: getTotalPortfolioValue
double TradingEngine::getTotalPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const {
    return portfolio_.getPortfolioValue(marketPrices);
}



// new code addded on 2024.10.28

// User Place Order (modified function)
void TradingEngine::userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style, double price,
                                   double stopLossPrice, double takeProfitPrice,
                                   const std::unordered_map<std::string, double>& marketPrices) {
    Order order(type, style, symbol, 100, price, stopLossPrice, takeProfitPrice);

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

// end of new code added on 2024.10.28

