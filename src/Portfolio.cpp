////
//// Created by jack on 9/10/24.
////
//#include "Portfolio.h"
//
//Portfolio::Portfolio(double initialCash)
//        : cashBalance_(initialCash), realizedPnL_(0.0) {}
//
//void Portfolio::updatePortfolio(const std::vector<Trade>& trades) {
//    for (const auto& trade : trades) {
//        int quantity = trade.getQuantity();
//        double price = trade.getPrice();
//        std::string symbol = trade.getSymbol();
//
//        if (trade.getType() == OrderType::Buy) {
//            cashBalance_ -= quantity * price;
//            positions_[symbol] += quantity;
//        } else if (trade.getType() == OrderType::Sell) {
//            cashBalance_ += quantity * price;
//            positions_[symbol] -= quantity;
//            // Update realized PnL if needed
//        }
//        tradeHistory_.push_back(trade);
//    }
//}
//
//double Portfolio::getCashBalance() const {
//    return cashBalance_;
//}
//
//double Portfolio::getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const {
//    double unrealizedPnL = 0.0;
//    for (const auto& position : positions_) {
//        const std::string& symbol = position.first;
//        int quantity = position.second;
//        auto it = marketPrices.find(symbol);
//        if (it != marketPrices.end()) {
//            double marketPrice = it->second;
//            // For simplicity, assume average cost is zero
//            unrealizedPnL += quantity * marketPrice;
//        }
//    }
//    return unrealizedPnL;
//}
//
//double Portfolio::getRealizedPnL() const {
//    return realizedPnL_;
//}
//
//int Portfolio::getPosition(const std::string& symbol) const {
//    auto it = positions_.find(symbol);
//    return (it != positions_.end()) ? it->second : 0;
//}
//
//


// Portfolio.cpp

#include "Portfolio.h"
#include "OrderExecutor.h"
#include <iostream>
#include <stdexcept>
#include "Trade.h" // Ensure full definition is included
#include "Order.h"
#include <vector>

// Constructor
Portfolio::Portfolio(double initialCash)
        : cashBalance_(initialCash), realizedPnL_(0.0) {}

// Update the portfolio with executed trades
void Portfolio::updatePortfolio(const std::vector<Trade>& trades) {
    for (const auto& trade : trades) {
        int quantity = trade.getQuantity();
        double price = trade.getPrice();
        std::string symbol = trade.getSymbol();

        if (trade.getType() == OrderType::Buy) {
            cashBalance_ -= quantity * price;
            positions_[symbol] += quantity;
        } else if (trade.getType() == OrderType::Sell) {
            cashBalance_ += quantity * price;
            positions_[symbol] -= quantity;
            // Update realized P&L if needed
        }
        tradeHistory_.push_back(trade);
    }
}

// Place an order and process it based on the market prices
void Portfolio::placeOrder(const Order& order, const std::unordered_map<std::string, double>& marketPrices) {
    if (order.getStyle() == OrderStyle::Market) {
        // Immediate execution
        executeOrder(order);
    } else if (order.getStyle() == OrderStyle::Limit) {
        // Check if the limit price condition is met
        auto it = marketPrices.find(order.getSymbol());
        if (it != marketPrices.end()) {
            double marketPrice = it->second;
            if ((order.getType() == OrderType::Buy && marketPrice <= order.getPrice()) ||
                (order.getType() == OrderType::Sell && marketPrice >= order.getPrice())) {
                // Limit price condition met
                executeOrder(order);
            } else {
                // Store the order as pending
                pendingOrders_.push_back(order);
                std::cout << "Order placed in pending orders.\n";
            }
        } else {
            throw std::runtime_error("Market price not available for symbol: " + order.getSymbol());
        }
    }
}

// Execute an order immediately
void Portfolio::executeOrder(const Order& order) {
    OrderExecutor executor;
    std::vector<Order> orders = { order };
    std::vector<Trade> trades = executor.executeOrders(orders);
    updatePortfolio(trades);

    std::cout << "Order executed: " << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
              << " " << order.getQuantity() << " shares of " << order.getSymbol()
              << " at $" << order.getPrice() << "\n";
}

// Process pending limit orders against the current market prices
void Portfolio::processPendingOrders(const std::unordered_map<std::string, double>& marketPrices) {
    auto it = pendingOrders_.begin();
    while (it != pendingOrders_.end()) {
        const Order& order = *it;
        auto marketIt = marketPrices.find(order.getSymbol());
        if (marketIt != marketPrices.end()) {
            double marketPrice = marketIt->second;
            bool execute = false;
            if ((order.getType() == OrderType::Buy && marketPrice <= order.getPrice()) ||
                (order.getType() == OrderType::Sell && marketPrice >= order.getPrice())) {
                execute = true;
            }

            if (execute) {
                executeOrder(order);
                it = pendingOrders_.erase(it); // Remove the order from pending orders
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
}

// Get the current cash balance
double Portfolio::getCashBalance() const {
    return cashBalance_;
}

// Get the unrealized P&L based on current market prices
double Portfolio::getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const {
    double unrealizedPnL = 0.0;
    for (const auto& position : positions_) {
        const std::string& symbol = position.first;
        int quantity = position.second;
        auto it = marketPrices.find(symbol);
        if (it != marketPrices.end()) {
            double marketPrice = it->second;
            // For simplicity, assume average cost is zero
            unrealizedPnL += quantity * marketPrice;
        }
    }
    return unrealizedPnL;
}

// Get the position (number of shares) for a specific symbol
int Portfolio::getPosition(const std::string& symbol) const {
    auto it = positions_.find(symbol);
    return (it != positions_.end()) ? it->second : 0;
}

// Get all positions in the portfolio
const std::unordered_map<std::string, int>& Portfolio::getPositions() const {
    return positions_;
}

// Additional methods (if any)...