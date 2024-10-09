//
// Created by jack on 9/10/24.
//

#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Order.h"
#include "Trade.h"
#include <string>
#include <unordered_map>
#include <vector>

class Portfolio {
public:
    Portfolio(double initialCash);

    // Methods for user interaction
    void placeOrder(const Order& order);
    const std::vector<Trade>& getTradeHistory() const;

    void updatePortfolio(const std::vector<Trade>& trades);

    // Getters
    double getCashBalance() const;
    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;
    double getRealizedPnL() const;
    int getPosition(const std::string& symbol) const;

private:
    double cashBalance_;
    double realizedPnL_;
    std::unordered_map<std::string, int> positions_; // symbol -> quantity
    std::vector<Trade> tradeHistory_;
};

#endif // PORTFOLIO_H