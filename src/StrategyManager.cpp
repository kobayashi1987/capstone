//
// Created by jack on 7/10/24.
//

#include "StrategyManager.h"
#include <stdexcept>

StrategyManager::StrategyManager() : strategy_(nullptr) {}

// Set a single strategy
void StrategyManager::setStrategy(std::unique_ptr<TradingStrategy> strategy) {
    strategy_ = std::move(strategy);
}

// Add a strategy to the list
void StrategyManager::addStrategy(std::unique_ptr<TradingStrategy> strategy) {
    strategies_.push_back(std::move(strategy));
}

// Clear all strategies
void StrategyManager::clearStrategies() {
    strategy_.reset();
    strategies_.clear();
    allSignals_.clear();
}

// Execute the strategy or strategies
void StrategyManager::executeStrategy(const std::vector<double>& prices) {
    if (strategy_) {
        // Execute single strategy
        strategy_->generateSignals(prices);
    } else if (!strategies_.empty()) {
        // Execute multiple strategies
        allSignals_.clear();
        for (const auto& strat : strategies_) {
            strat->generateSignals(prices);
            allSignals_.push_back(strat->getSignals());
        }
    } else {
        throw std::runtime_error("No strategy set to execute.");
    }
}

// Retrieve signals from single strategy
const std::vector<int>& StrategyManager::getSignals() const {
    if (strategy_) {
        return strategy_->getSignals();
    } else {
        throw std::runtime_error("No single strategy has been set.");
    }
}

// Retrieve signals from all strategies
const std::vector<std::vector<int>>& StrategyManager::getAllSignals() const {
    if (!allSignals_.empty()) {
        return allSignals_;
    } else {
        throw std::runtime_error("No strategies have been executed.");
    }
}