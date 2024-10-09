//
// Created by jack on 9/10/24.
//

#include "MarketDataFeed.h"
#include <cstdlib>
#include <ctime>

MarketDataFeed::MarketDataFeed() {
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initialize some stock symbols and prices
    symbols_ = { "AAPL", "GOOGL", "MSFT", "AMZN", "FB" };
    for (const auto& symbol : symbols_) {
        marketPrices_[symbol] = 100.0 + (std::rand() % 1000) / 10.0; // Random price between $100 and $200
    }
}

void MarketDataFeed::updatePrices() {
    simulatePriceChanges();
}

double MarketDataFeed::getPrice(const std::string& symbol) const {
    auto it = marketPrices_.find(symbol);
    if (it != marketPrices_.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Price not available for symbol: " + symbol);
    }
}

const std::unordered_map<std::string, double>& MarketDataFeed::getPrices() const {
    return marketPrices_;
}

void MarketDataFeed::simulatePriceChanges() {
    for (auto& pair : marketPrices_) {
        double change = ((std::rand() % 200) - 100) / 100.0; // Random change between -1 and 1
        pair.second += change;
        if (pair.second < 1.0) {
            pair.second = 1.0; // Ensure price doesn't go below $1
        }
    }
}