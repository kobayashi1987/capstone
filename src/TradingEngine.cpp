//
// Created by jack on 9/10/24.
//
#include "TradingEngine.h"
#include "Order.h"
#include <iostream>

TradingEngine::TradingEngine(double initialCash)
        : portfolio_(initialCash) {}

void TradingEngine::setStrategy(std::unique_ptr<TradingStrategy> strategy) {
    strategy_ = std::move(strategy);
}

void TradingEngine::executeStrategy(const std::vector<double>& prices, const std::string& symbol) {
    if (strategy_) {
        std::vector<Order> orders = strategy_->generateOrders(prices, symbol);
        std::vector<Trade> trades = executor_.executeOrders(orders);
        portfolio_.updatePortfolio(trades);
    }
}


//void TradingEngine::userBuy(const std::string& symbol, int quantity, double price) {
//    Order order(OrderType::Buy, symbol, quantity, price);
//    portfolio_.placeOrder(order);
//}
//
//void TradingEngine::userSell(const std::string& symbol, int quantity, double price) {
//    Order order(OrderType::Sell, symbol, quantity, price);
//    portfolio_.placeOrder(order);
//}

// above two parts of code are replaced by the following code
void TradingEngine::userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style, int quantity, double price, const std::unordered_map<std::string, double>& marketPrices) {
    try {
        Order order(type, style, symbol, quantity, price);
        portfolio_.placeOrder(order, marketPrices);
    } catch (const std::exception& e) {
        std::cerr << "Error executing order: " << e.what() << "\n";
    }
}
double TradingEngine::getCashBalance() const {
    return portfolio_.getCashBalance();
}

int TradingEngine::getPosition(const std::string& symbol) const {
    return portfolio_.getPosition(symbol);
}

double TradingEngine::getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const {
    return portfolio_.getUnrealizedPnL(marketPrices);
}

// newly added : 2024 Oct 9a
const std::unordered_map<std::string, int>& TradingEngine::getPositions() const {
    return portfolio_.getPositions();
}

void TradingEngine::processPendingOrders() {
    portfolio_.processPendingOrders(marketDataFeed_.getPrices());
}

void TradingEngine::processPendingOrders(const std::unordered_map<std::string, double>& marketPrices) {
    portfolio_.processPendingOrders(marketPrices);
}