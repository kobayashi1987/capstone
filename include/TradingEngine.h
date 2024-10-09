//
// Created by jack on 9/10/24.
//

#ifndef TRADINGENGINE_H
#define TRADINGENGINE_H

#include "TradingStrategy.h"
#include "OrderExecutor.h"
#include "Portfolio.h"
#include <memory>

class TradingEngine {
public:
    TradingEngine(double initialCash);

    // Set and execute strategy
    void setStrategy(std::unique_ptr<TradingStrategy> strategy);
    void executeStrategy(const std::vector<double>& prices, const std::string& symbol);

    // User interaction methods
    void userBuy(const std::string& symbol, int quantity, double price);
    void userSell(const std::string& symbol, int quantity, double price);

    // Portfolio access
    double getCashBalance() const;
    int getPosition(const std::string& symbol) const;
    double getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const;

private:
    std::unique_ptr<TradingStrategy> strategy_;
    OrderExecutor executor_;
    Portfolio portfolio_;
};

#endif // TRADINGENGINE_H