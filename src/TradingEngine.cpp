//
// Created by jack on 9/10/24.
//
#include "TradingEngine.h"

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

void TradingEngine::userBuy(const std::string& symbol, int quantity, double price) {
    Order order(OrderType::Buy, symbol, quantity, price);
    portfolio_.placeOrder(order);
}

void TradingEngine::userSell(const std::string& symbol, int quantity, double price) {
    Order order(OrderType::Sell, symbol, quantity, price);
    portfolio_.placeOrder(order);
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