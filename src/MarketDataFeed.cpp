////
//// Created by jack on 9/10/24.
////
//
//#include "MarketDataFeed.h"
//#include <cstdlib>
//#include <ctime>
//
//MarketDataFeed::MarketDataFeed() {
//    // Initialize random seed
//    std::srand(static_cast<unsigned int>(std::time(nullptr)));
//
//    // Initialize some stock symbols and prices
//    symbols_ = { "AAPL", "GOOGL", "MSFT", "AMZN", "FB" };
//    for (const auto& symbol : symbols_) {
//        marketPrices_[symbol] = 100.0 + (std::rand() % 1000) / 10.0; // Random price between $100 and $200
//    }
//}
//
//void MarketDataFeed::updatePrices() {
//    simulatePriceChanges();
//}
//
//double MarketDataFeed::getPrice(const std::string& symbol) const {
//    auto it = marketPrices_.find(symbol);
//    if (it != marketPrices_.end()) {
//        return it->second;
//    } else {
//        throw std::runtime_error("Price not available for symbol: " + symbol);
//    }
//}
//
//const std::unordered_map<std::string, double>& MarketDataFeed::getPrices() const {
//    return marketPrices_;
//}
//
//void MarketDataFeed::simulatePriceChanges() {
//    for (auto& pair : marketPrices_) {
//        double change = ((std::rand() % 200) - 100) / 100.0; // Random change between -1 and 1
//        pair.second += change;
//        if (pair.second < 1.0) {
//            pair.second = 1.0; // Ensure price doesn't go below $1
//        }
//    }
//}


// src/MarketDataFeed.cpp

#include "MarketDataFeed.h"
#include <stdexcept>
#include <cstdlib> // For std::rand
#include <ctime>   // For std::time

// Constructor: Initializes the MarketDataFeed with a list of stock symbols and their initial prices.
MarketDataFeed::MarketDataFeed(const std::vector<std::string>& symbols, const std::unordered_map<std::string, double>& initialPrices) {
    // Initialize the prices_ map with the provided initial prices.
    for (const auto& symbol : symbols) {
        auto it = initialPrices.find(symbol);
        if (it != initialPrices.end()) {
            // If an initial price is provided for the symbol, use it.
            prices_[symbol] = it->second;
        } else {
            // If no initial price is provided, set a default price (e.g., $100.0).
            prices_[symbol] = 100.0;
        }
    }
}

// Method to get the current price of a specific symbol.
double MarketDataFeed::getPrice(const std::string& symbol) const {
    auto it = prices_.find(symbol);
    if (it != prices_.end()) {
        return it->second;
    } else {
        // Throw an exception if the symbol is not found.
        throw std::runtime_error("Price not available for symbol: " + symbol);
    }
}

// Method to get all current market prices (const reference).
const std::unordered_map<std::string, double>& MarketDataFeed::getPrices() const {
    return prices_;
}

// Method to get mutable access to prices (used for simulating price updates).
std::unordered_map<std::string, double>& MarketDataFeed::getPricesMutable() {
    return prices_;
}

// Method to update market prices (simulates price changes by adding random fluctuations).
void MarketDataFeed::updatePrices() {
    for (auto& pair : prices_) {
        // Simulate a price fluctuation between -1% and +1%.
        // Generate a random number between -100 and +100.
        double fluctuationPercentage = ((std::rand() % 201) - 100) / 10000.0; // Random percentage between -0.01 and +0.01
        double fluctuation = pair.second * fluctuationPercentage;
        pair.second += fluctuation;
        if (pair.second < 0.01) {
            // Ensure the price doesn't go below $0.01.
            pair.second = 0.01;
        }
    }
}