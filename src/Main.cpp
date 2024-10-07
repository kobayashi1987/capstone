//
// Created by jack on 7/10/24.
//

// src/main.cpp

#include <iostream>
#include "MovingAverageCrossoverStrategy.h"

int main() {
    // Sample price data (e.g., cryptocurrency closing prices)
    std::vector<double> prices = {
            100.0, 101.5, 102.3, 103.7, 104.1,
            103.5, 102.8, 101.2, 100.5, 99.8,
            100.2, 101.0, 102.5, 103.0, 104.5
    };

    // Instantiate the strategy with short window = 3 and long window = 5
    MovingAverageCrossoverStrategy strategy(3, 5);

    // Generate trading signals based on the price data
    strategy.generateSignals(prices);

    // Retrieve the generated signals
    const std::vector<int>& signals = strategy.getSignals();

    // Output the results
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

    return 0;
}