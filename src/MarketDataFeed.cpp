//////
////// Created by jack on 9/10/24.
//////
//
//
//// src/MarketDataFeed.cpp
//
//#include "MarketDataFeed.h"
//#include <stdexcept>
//#include <cstdlib> // For std::rand
//#include <ctime>   // For std::time
//
//// Constructor: Initializes the MarketDataFeed with a list of stock symbols and their initial prices.
//MarketDataFeed::MarketDataFeed(const std::vector<std::string>& symbols, const std::unordered_map<std::string, double>& initialPrices) {
//    // Initialize the prices_ map with the provided initial prices.
//    for (const auto& symbol : symbols) {
//        auto it = initialPrices.find(symbol);
//        if (it != initialPrices.end()) {
//            // If an initial price is provided for the symbol, use it.
//            prices_[symbol] = it->second;
//        } else {
//            // If no initial price is provided, set a default price (e.g., $100.0).
//            prices_[symbol] = 100.0;
//        }
//    }
//}
//
//// Method to get the current price of a specific symbol.
//double MarketDataFeed::getPrice(const std::string& symbol) const {
//    auto it = prices_.find(symbol);
//    if (it != prices_.end()) {
//        return it->second;
//    } else {
//        // Throw an exception if the symbol is not found.
//        throw std::runtime_error("Price not available for symbol: " + symbol);
//    }
//}
//
//// Method to get all current market prices (const reference).
//const std::unordered_map<std::string, double>& MarketDataFeed::getPrices() const {
//    return prices_;
//}
//
//// Method to get mutable access to prices (used for simulating price updates).
//std::unordered_map<std::string, double>& MarketDataFeed::getPricesMutable() {
//    return prices_;
//}
//
//// Method to update market prices (simulates price changes by adding random fluctuations).
//void MarketDataFeed::updatePrices() {
//    for (auto& pair : prices_) {
//        // Simulate a price fluctuation between -1% and +1%.
//        // Generate a random number between -100 and +100.
//        double fluctuationPercentage = ((std::rand() % 201) - 100) / 10000.0; // Random percentage between -0.01 and +0.01
//        double fluctuation = pair.second * fluctuationPercentage;
//        pair.second += fluctuation;
//        if (pair.second < 0.01) {
//            // Ensure the price doesn't go below $0.01.
//            pair.second = 0.01;
//        }
//    }
//}

// code added on 2024.10.23

// src/MarketDataFeed.cpp

#include "MarketDataFeed.h"

#include <stdexcept>
#include <cstdlib> // For std::rand
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


// Constructor: Initializes the MarketDataFeed with a list of stock symbols and their initial prices.
MarketDataFeed::MarketDataFeed(const std::vector<std::string>& symbols, const std::unordered_map<std::string, double>& initialPrices)
        : stopUpdates_(false) {
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

// Destructor: Ensures that the price update thread is stopped.
MarketDataFeed::~MarketDataFeed() {
    stopPriceUpdates();
}

// Method to get the current price of a specific symbol.
double MarketDataFeed::getPrice(const std::string& symbol) const {
    std::lock_guard<std::mutex> lock(pricesMutex_);
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
    // No need to lock mutex for const reference return, but ensure that prices_ is not modified concurrently.
    return prices_;
}

// Method to start live price updates.
void MarketDataFeed::startPriceUpdates() {
    // If the update thread is already running, do not start a new one.
    if (priceUpdateThread_.joinable()) {
        return;
    }
    stopUpdates_ = false;
    priceUpdateThread_ = std::thread(&MarketDataFeed::priceUpdateLoop, this);
}

// Method to stop live price updates.
void MarketDataFeed::stopPriceUpdates() {
    stopUpdates_ = true;
    if (priceUpdateThread_.joinable()) {
        priceUpdateThread_.join();
    }
}

// Method that runs in a separate thread to periodically update prices.
void MarketDataFeed::priceUpdateLoop() {
    while (!stopUpdates_) {
        // Update prices
        updatePrices();

        // Sleep for 1 second before next update
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// Method to update market prices (simulates price changes by adding random fluctuations).
void MarketDataFeed::updatePrices() {
    std::lock_guard<std::mutex> lock(pricesMutex_);
    for (auto& pair : prices_) {
        // Simulate a price fluctuation between -1% and +1%.
        // Generate a random number between -100 and +100.
        double fluctuationPercentage = ((std::rand() % 201) - 100) / 10000.0; // Random percentage between -1% and +1%
        double fluctuation = pair.second * fluctuationPercentage;
        pair.second += fluctuation;
        if (pair.second < 0.01) {
            // Ensure the price doesn't go below $0.01.
            pair.second = 0.01;
        }
    }
}