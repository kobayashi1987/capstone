//
// Created by jack on 9/10/24.
//

//#ifndef MARKETDATAFEED_H
//#define MARKETDATAFEED_H
//
//#include <unordered_map>
//#include <string>
//#include <vector>
//
//class MarketDataFeed {
//public:
//    MarketDataFeed();
//
//    void updatePrices();
//    double getPrice(const std::string& symbol) const;
//    const std::unordered_map<std::string, double>& getPrices() const;
//
//private:
//    std::unordered_map<std::string, double> marketPrices_;
//    std::vector<std::string> symbols_;
//
//    void simulatePriceChanges();
//};
//
//#endif // MARKETDATAFEED_H


// include/MarketDataFeed.h

#ifndef MARKETDATAFEED_H
#define MARKETDATAFEED_H

#include <string>
#include <unordered_map>
#include <vector>

// The MarketDataFeed class provides simulated market data for a list of stock symbols.
// It manages the current prices of stocks and allows updating prices to simulate market movements.

class MarketDataFeed {
public:
    // Constructor: Initializes the MarketDataFeed with a list of stock symbols and their initial prices.
    // Parameters:
    // - symbols: A vector of stock symbols to track.
    // - initialPrices: A map of initial prices for the stock symbols.
    MarketDataFeed(const std::vector<std::string>& symbols, const std::unordered_map<std::string, double>& initialPrices);

    // Method to get the current price of a specific symbol.
    // Parameters:
    // - symbol: The stock symbol to get the price for.
    // Returns:
    // - The current price of the stock.
    // Throws:
    // - std::runtime_error if the symbol is not found.
    double getPrice(const std::string& symbol) const;

    // Method to get all current market prices (const reference).
    // Returns:
    // - A constant reference to the map of stock symbols to their current prices.
    const std::unordered_map<std::string, double>& getPrices() const;

    // Method to get mutable access to prices (used for simulating price updates).
    // Returns:
    // - A reference to the map of stock symbols to their current prices.
    std::unordered_map<std::string, double>& getPricesMutable();

    // Method to update market prices (simulates price changes by adding random fluctuations).
    void updatePrices();

private:
    // Map of stock symbols to their current prices.
    std::unordered_map<std::string, double> prices_;
};

#endif // MARKETDATAFEED_H