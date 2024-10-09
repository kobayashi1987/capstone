//
// Created by jack on 7/10/24.
//

#ifndef STRATEGYMANAGER_H
#define STRATEGYMANAGER_H

#include "TradingStrategy.h"
#include <memory>
#include <vector>

class StrategyManager {
public:
    StrategyManager();

    // Methods to manage strategies
    void setStrategy(std::unique_ptr<TradingStrategy> strategy);
    void addStrategy(std::unique_ptr<TradingStrategy> strategy);
    void clearStrategies();

    // Methods to execute strategies
    void executeStrategy(const std::vector<double>& prices);

    // Methods to retrieve signals
    const std::vector<int>& getSignals() const; // For single strategy
    const std::vector<std::vector<int>>& getAllSignals() const; // For multiple strategies

private:
    // Single strategy
    std::unique_ptr<TradingStrategy> strategy_;

    // Multiple strategies
    std::vector<std::unique_ptr<TradingStrategy>> strategies_;
    std::vector<std::vector<int>> allSignals_;
};

#endif // STRATEGYMANAGER_H