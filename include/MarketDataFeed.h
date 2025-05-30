
// code added on 2024.10.23

#ifndef MARKETDATAFEED_H
#define MARKETDATAFEED_H

#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

// The MarketDataFeed class provides simulated live market data for a list of stock symbols.
// It manages the current prices of stocks and updates prices at regular intervals to simulate live data.

class MarketDataFeed {
public:

    // Default Constructor
    MarketDataFeed();

    // Parameterized Constructor
    MarketDataFeed(const std::string& apiKey, const std::string& dataSource);

    // Constructor: Initializes the MarketDataFeed with a list of stock symbols and their initial prices.
    MarketDataFeed(const std::vector<std::string>& symbols, const std::unordered_map<std::string, double>& initialPrices);

    // Destructor: Ensures that the price update thread is stopped.
    ~MarketDataFeed();

    // Method to get the current price of a specific symbol.
    double getPrice(const std::string& symbol) const;

    // Method to get all current market prices (const reference).
    const std::unordered_map<std::string, double>& getPrices() const;

    // Method to start live price updates.
    void startPriceUpdates();

    // Method to stop live price updates.
    void stopPriceUpdates();

    // Update a single market price
    void updatePrice(const std::string& symbol, double price);


private:
    // Method to update market prices (simulates price changes by adding random fluctuations).
    void updatePrices();

    // Method that runs in a separate thread to periodically update prices.
    void priceUpdateLoop();

    // Map of stock symbols to their current prices.
    std::unordered_map<std::string, double> prices_;

    // Mutex to protect access to prices_.
    mutable std::mutex pricesMutex_;

    // Thread for updating prices.
    std::thread priceUpdateThread_;

    // Atomic flag to control the price update loop.
    std::atomic<bool> stopUpdates_;
};

#endif // MARKETDATAFEED_H