#include <iostream>
#include "MovingAverageCrossoverStrategy.h"
#include "RSIStrategy.h" // Assuming you have implemented this strategy
#include "StrategyManager.h"

int main() {
    // Sample price data (e.g., cryptocurrency closing prices)
    std::vector<double> prices = {
            100.0, 101.5, 102.3, 103.7, 104.1,
            103.5, 102.8, 101.2, 100.5, 99.8,
            100.2, 101.0, 102.5, 103.0, 104.5
    };

    // Create a StrategyManager instance
    StrategyManager manager;

    // **Option 1: Using a Single Strategy**

    // Instantiate a Moving Average Crossover Strategy
    auto macStrategy = std::make_unique<MovingAverageCrossoverStrategy>(3, 5);

    // Set the strategy in the manager
    manager.setStrategy(std::move(macStrategy));

    // Execute the strategy
    manager.executeStrategy(prices);

    // Retrieve and display the signals
    const std::vector<int>& signals = manager.getSignals();

    std::cout << "Single Strategy (Moving Average Crossover) Signals:\n";
    std::cout << "Date\tPrice\tSignal\n";
    for (size_t i = 0; i < prices.size(); ++i) {
        std::string signalText;
        if (signals[i] == 1) {
            signalText = "Buy";
        } else if (signals[i] == -1) {
            signalText = "Sell";
        } else {
            signalText = "Hold";
        }
        std::cout << i + 1 << "\t" << prices[i] << "\t" << signalText << "\n";
    }

    // **Option 2: Using Multiple Strategies**

    // Clear the previous strategy
    manager.clearStrategies();

    // Add multiple strategies
    manager.addStrategy(std::make_unique<MovingAverageCrossoverStrategy>(3, 5));
    manager.addStrategy(std::make_unique<RSIStrategy>(14)); // Example RSI strategy

    // Execute all strategies
    manager.executeStrategy(prices);

    // Retrieve and display the signals from all strategies
    const auto& allSignals = manager.getAllSignals();

    std::cout << "\nMultiple Strategies Signals:\n";

    for (size_t stratIndex = 0; stratIndex < allSignals.size(); ++stratIndex) {
        std::cout << "Strategy " << stratIndex + 1 << " Signals:\n";
        std::cout << "Date\tPrice\tSignal\n";
        for (size_t i = 0; i < prices.size(); ++i) {
            std::string signalText;
            int signal = allSignals[stratIndex][i];
            if (signal == 1) {
                signalText = "Buy";
            } else if (signal == -1) {
                signalText = "Sell";
            } else {
                signalText = "Hold";
            }
            std::cout << i + 1 << "\t" << prices[i] << "\t" << signalText << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}