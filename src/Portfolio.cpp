//
// Created by jack on 9/10/24.
//
#include "Portfolio.h"
#include "OrderExecutor.h"



Portfolio::Portfolio(double initialCash)
        : cashBalance_(initialCash), realizedPnL_(0.0) {}

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
            // Update realized PnL if needed
        }
        tradeHistory_.push_back(trade);
    }
}

double Portfolio::getCashBalance() const {
    return cashBalance_;
}

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

double Portfolio::getRealizedPnL() const {
    return realizedPnL_;
}

int Portfolio::getPosition(const std::string& symbol) const {
    auto it = positions_.find(symbol);
    return (it != positions_.end()) ? it->second : 0;
}


void Portfolio::placeOrder(const Order& order) {
    OrderExecutor executor;
    std::vector<Order> orders = { order };
    std::vector<Trade> trades = executor.executeOrders(orders);
    updatePortfolio(trades);
}

const std::vector<Trade>& Portfolio::getTradeHistory() const {
    return tradeHistory_;
}

