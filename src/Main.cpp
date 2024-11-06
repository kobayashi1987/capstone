// main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdlib>  // For std::rand(), std::srand()
#include <ctime>    // For std::time()
#include <stdexcept> // For std::exception
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <limits>  // For std::numeric_limits
#include <iomanip> // For std::setprecision

// added 2024.11.02
#include <unistd.h>     // For getcwd
#include <limits.h>     // For PATH_MAX

//#include <Python.h> // For Python C API

// For JSON parsing
#include "json.hpp"
using json = nlohmann::json;

namespace fs = std::filesystem;


// Include all the necessary headers
#include "Utils.h"
#include "TradingEngine.h"
#include "MarketDataFeed.h"
#include "Order.h"
#include "Portfolio.h"
#include "TradingStrategy.h"
#include "MovingAverageCrossoverStrategy.h"
#include "Trade.h"
#include "RSIStrategy.h"
#include "OrderExecutor.h"
#include "StrategyManager.h"
#include "MomentumStrategy.h"
#include "BreakoutStrategy.h"
#include "MeanReversionStrategy.h"
#include "DataPersistence.h" // Assumed to handle JSON serialization
#include "ProfitLoss.h" // Newly added


// Template function to safely capture user input of various types
template <typename T>
T getInput(const std::string& prompt) {
    T value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            // Clear any remaining input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
            // Clear error flags and ignore invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}


// Function to get a string input (handles spaces)
std::string getStringInput(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        if (std::getline(std::cin, value)) {
            if (!value.empty()) {
                return value;
            }
            else {
                std::cout << "Input cannot be empty. Please try again.\n";
            }
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
        }
    }
}


// Seed the random number generator
void initializeRandomGenerator() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}


// Function to display the main menu

void displayMainMenu() {
    std::cout << "\n=== Trading Engine Menu ===\n";
    std::cout << "1. Place a New Order\n";
    std::cout << "2. View Portfolio\n";
    std::cout << "3. View Trade History\n";
    std::cout << "4. View Pending Orders\n";
    std::cout << "5. Update Market Prices\n";
    std::cout << "6. View Live Prices\n";
    std::cout << "7. View Market Prices\n";
    std::cout << "8. Execute Trading Strategies\n";
    std::cout << "9. Save and Exit\n";
    std::cout << "Enter your choice: ";
}

// Forward declarations of strategy execution functions
// Function to display live prices every 1 seconds
void viewLivePrices(TradingEngine& engine, MarketDataFeed& marketDataFeed);
void viewPendingOrders(const TradingEngine& engine);
void viewMarketPrices(const MarketDataFeed& marketDataFeed);
void placeOrder(TradingEngine& engine, const MarketDataFeed& marketDataFeed, OrderType);
void viewPortfolio(const TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeMovingAverageCrossoverStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeRSIStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeMomentumStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeBreakoutStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeMeanReversionStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void viewTradeHistory(const TradingEngine& engine);
void updateMarketPrices(std::unordered_map<std::string, double>& marketPrices, MarketDataFeed& marketDataFeed, TradingEngine& engine);
void executeTradingStrategies(TradingEngine& engine, MarketDataFeed& marketDataFeed);

// BELOW IS NEWLY MOVED functions from main.cpp
// Function to view the portfolio
void viewPortfolio(const TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::cout << "\n=== Portfolio ===\n";
    double cash = engine.getCashBalance();
    std::cout << "Cash Balance: $" << cash << "\n";

    const auto& positions = engine.getPositions();
    if (positions.empty()) {
        std::cout << "No open positions.\n";
    }
    else {
        std::cout << "\n--- Open Positions ---\n";
        for (const auto& [symbol, position] : positions) {
            double marketPrice = 0.0;
            auto prices = marketDataFeed.getPrices();
            auto it = prices.find(symbol);
            if (it != prices.end()) {
                marketPrice = it->second;
            }
            else {
                std::cout << "Market price for symbol " << symbol << " not found.\n";
                marketPrice = position.averagePrice; // Fallback
            }
            double positionValue = position.quantity * marketPrice;
            double unrealizedPnL = (marketPrice - position.averagePrice) * position.quantity;
            std::cout << "Symbol: " << symbol
                      << ", Quantity: " << position.quantity
                      << ", Average Price: $" << position.averagePrice
                      << ", Market Price: $" << marketPrice
                      << ", Position Value: $" << positionValue
                      << ", Unrealized P&L: $" << unrealizedPnL << "\n";
        }
    }

    double totalPortfolioValue = engine.getTotalPortfolioValue(marketDataFeed.getPrices());
    double unrealizedPnL = engine.getPortfolio().getUnrealizedPnL(marketDataFeed.getPrices());
    double peakValue = engine.getPortfolio().getPeakValue();
    double drawdown = engine.getPortfolio().getDrawdown() * 100; // Convert to percentage

    std::cout << "\n--- Summary ---\n";
    std::cout << "Total Portfolio Value: $" << totalPortfolioValue << "\n";
    std::cout << "Unrealized P&L: $" << unrealizedPnL << "\n";
    std::cout << "Peak Value: $" << peakValue << "\n";
    std::cout << "Current Drawdown: " << drawdown << "%\n";
}

// New Function to execute Mean Reversion Strategy
void executeMeanReversionStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    int lookbackPeriod;
    double deviationThreshold;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;
    std::cout << "Enter the look-back period for mean calculation: ";
    std::cin >> lookbackPeriod;
    std::cout << "Enter the deviation threshold (e.g., 0.05 for 5%): ";
    std::cin >> deviationThreshold;

    if (lookbackPeriod <= 0 || deviationThreshold <= 0) {
        std::cout << "Look-back period and deviation threshold must be positive numbers.\n";
        return;
    }

    auto strategy = std::make_unique<MeanReversionStrategy>(lookbackPeriod, deviationThreshold);

    // Generate dummy historical prices simulating mean reversion
    std::vector<double> prices(200);
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }
    prices[0] = currentPrice;
    double meanPrice = currentPrice;
    for (size_t i = 1; i < prices.size(); ++i) {
        double randomNoise = ((std::rand() % 200) - 100) / 50.0; // Random noise between -2 and +2
        prices[i] = prices[i - 1] + randomNoise;
        // Gradually pull prices back towards the mean
        prices[i] += (meanPrice - prices[i]) * 0.1;
        if (prices[i] < 0) {
            prices[i] = 0.01;
        }
    }

    // Generate signals
    try {
        strategy->generateSignals(prices);
    } catch (const std::exception& e) {
        std::cout << "Error generating signals: " << e.what() << "\n";
        return;
    }

    // Retrieve signals and mean values
    const auto& signals = strategy->getSignals();
    const auto& meanValues = strategy->getMeanValues();

    // Display signals and mean values
    std::cout << "\n=== Mean Reversion Signals ===\n";
    for (size_t i = lookbackPeriod; i < prices.size(); ++i) {
        double deviation = (prices[i] - meanValues[i]) / meanValues[i];
        std::cout << "Price: $" << prices[i]
                  << ", Mean: $" << meanValues[i]
                  << ", Deviation: " << deviation * 100 << "%"
                  << ", Signal: ";
        if (signals[i] == 1) {
            std::cout << "Buy\n";
        } else if (signals[i] == -1) {
            std::cout << "Sell\n";
        } else {
            std::cout << "Neutral\n";
        }
    }

    // Generate orders based on signals
    auto orders = strategy->generateOrders(prices, symbol);

    // Display and place orders
    std::cout << "\n=== Orders Placed ===\n";
    for (const auto& order : orders) {
        std::cout << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                  << " " << order.getQuantity() << " shares of "
                  << order.getSymbol() << " at $" << order.getPrice() << "\n";

        // Place the order with risk management (using default stop-loss and take-profit)
        try {
            engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
                                  order.getQuantity(), order.getPrice(),
                    /*stopLossPrice=*/0.0, /*takeProfitPrice=*/0.0,
                                  marketDataFeed.getPrices());
            std::cout << "Order placed successfully.\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error placing order: " << e.what() << "\n";
        }
    }

    std::cout << "\nMean Reversion strategy executed.\n";
}

// Function to view current market prices
void viewMarketPrices(const MarketDataFeed& marketDataFeed) {
    const auto& prices = marketDataFeed.getPrices();
    std::cout << "\n=== Current Market Prices ===\n";
    for (const auto& pair : prices) {
        std::cout << pair.first << ": $" << pair.second << "\n";
    }
}

// Function to execute the Moving Average Crossover Strategy
void executeMovingAverageCrossoverStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    int shortPeriod, longPeriod;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;
    std::cout << "Enter the short-term moving average period: ";
    std::cin >> shortPeriod;
    std::cout << "Enter the long-term moving average period: ";
    std::cin >> longPeriod;

    if (shortPeriod <= 0 || longPeriod <= 0 || shortPeriod >= longPeriod) {
        std::cout << "Invalid moving average periods.\n";
        return;
    }

    auto strategy = std::make_unique<MovingAverageCrossoverStrategy>(shortPeriod, longPeriod);

    // Generate dummy historical prices for testing
    std::vector<double> prices(200);
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }
    prices[0] = currentPrice;
    for (size_t i = 1; i < prices.size(); ++i) {
        double fluctuation = ((std::rand() % 100) - 50) / 10.0;
        prices[i] = prices[i - 1] + fluctuation;
        if (prices[i] < 0) {
            prices[i] = 0.01;
        }
    }

    // Generate signals
    try {
        strategy->generateSignals(prices);
    } catch (const std::exception& e) {
        std::cout << "Error generating signals: " << e.what() << "\n";
        return;
    }

    // Retrieve signals
    const auto& signals = strategy->getSignals();

    // Display signals
    std::cout << "\n=== Moving Average Crossover Signals ===\n";
    for (size_t i = longPeriod; i < prices.size(); ++i) {
        std::cout << "Price: $" << prices[i] << ", Signal: ";
        if (signals[i] == 1) {
            std::cout << "Buy\n";
        } else if (signals[i] == -1) {
            std::cout << "Sell\n";
        } else {
            std::cout << "Hold\n";
        }
    }

    // Generate orders
    auto orders = strategy->generateOrders(prices, symbol);

    // Display and place orders
    std::cout << "\n=== Orders Placed ===\n";
    for (const auto& order : orders) {
        std::cout << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                  << " " << order.getQuantity() << " shares of "
                  << order.getSymbol() << " at $" << order.getPrice() << "\n";

        // Place the order with risk management (using default stop-loss and take-profit)
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),order.getQuantity(),
                              order.getPrice(), /*stopLossPrice=*/0.0, /*takeProfitPrice=*/0.0,
                              marketDataFeed.getPrices());
    }

    std::cout << "\nMoving Average Crossover strategy executed.\n";
}

// Function to execute the RSI Strategy
void executeRSIStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    int period;
    double overboughtThreshold, oversoldThreshold;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;
    std::cout << "Enter the RSI period: ";
    std::cin >> period;
    std::cout << "Enter the overbought threshold (e.g., 70): ";
    std::cin >> overboughtThreshold;
    std::cout << "Enter the oversold threshold (e.g., 30): ";
    std::cin >> oversoldThreshold;

    if (period <= 0 || overboughtThreshold <= oversoldThreshold) {
        std::cout << "Invalid RSI parameters.\n";
        return;
    }

    auto strategy = std::make_unique<RSIStrategy>(period, overboughtThreshold, oversoldThreshold);

    // Generate dummy historical prices for testing
    std::vector<double> prices(200);
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }
    prices[0] = currentPrice;
    for (size_t i = 1; i < prices.size(); ++i) {
        double fluctuation = ((std::rand() % 100) - 50) / 10.0;
        prices[i] = prices[i - 1] + fluctuation;
        if (prices[i] < 0) {
            prices[i] = 0.01;
        }
    }

    // Generate signals
    try {
        strategy->generateSignals(prices);
    } catch (const std::exception& e) {
        std::cout << "Error generating signals: " << e.what() << "\n";
        return;
    }

    // Retrieve RSI values and signals
    const auto& rsiValues = strategy->getRSIValues();
    const auto& signals = strategy->getSignals();

    // Display RSI values and signals
    std::cout << "\n=== RSI Signals ===\n";
    for (size_t i = period; i < prices.size(); ++i) {
        std::cout << "Price: $" << prices[i]
                  << ", RSI: " << rsiValues[i]
                  << ", Signal: ";
        if (signals[i] == 1) {
            std::cout << "Buy\n";
        } else if (signals[i] == -1) {
            std::cout << "Sell\n";
        } else {
            std::cout << "Hold\n";
        }
    }

    // Generate orders
    auto orders = strategy->generateOrders(prices, symbol);

    // Display and place orders
    std::cout << "\n=== Orders Placed ===\n";
    for (const auto& order : orders) {
        std::cout << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                  << " " << order.getQuantity() << " shares of "
                  << order.getSymbol() << " at $" << order.getPrice() << "\n";

        // Place the order with risk management (using default stop-loss and take-profit)
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),order.getQuantity(),
                              order.getPrice(), /*stopLossPrice=*/0.0, /*takeProfitPrice=*/0.0,
                              marketDataFeed.getPrices());
    }

    std::cout << "\nRSI strategy executed.\n";
}

// Function to execute the Momentum Strategy
void executeMomentumStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    int lookbackPeriod;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;
    std::cout << "Enter the look-back period for momentum calculation: ";
    std::cin >> lookbackPeriod;

    if (lookbackPeriod <= 0) {
        std::cout << "Look-back period must be a positive integer.\n";
        return;
    }

    auto strategy = std::make_unique<MomentumStrategy>(lookbackPeriod);

    // Generate dummy historical prices for testing
    std::vector<double> prices(200);
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }
    prices[0] = currentPrice;
    for (size_t i = 1; i < prices.size(); ++i) {
        double fluctuation = ((std::rand() % 100) - 50) / 10.0;
        prices[i] = prices[i - 1] + fluctuation;
        if (prices[i] < 0) {
            prices[i] = 0.01;
        }
    }

    // Generate signals
    try {
        strategy->generateSignals(prices);
    } catch (const std::exception& e) {
        std::cout << "Error generating signals: " << e.what() << "\n";
        return;
    }

    // Retrieve signals
    const auto& signals = strategy->getSignals();

    // Display signals
    std::cout << "\n=== Momentum Signals ===\n";
    for (size_t i = lookbackPeriod; i < prices.size(); ++i) {
        std::cout << "Price: $" << prices[i] << ", Signal: ";
        if (signals[i] == 1) {
            std::cout << "Buy\n";
        } else if (signals[i] == -1) {
            std::cout << "Sell\n";
        } else {
            std::cout << "Hold\n";
        }
    }

    // Generate orders
    auto orders = strategy->generateOrders(prices, symbol);

    // Display and place orders
    std::cout << "\n=== Orders Placed ===\n";
    for (const auto& order : orders) {
        std::cout << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                  << " " << order.getQuantity() << " shares of "
                  << order.getSymbol() << " at $" << order.getPrice() << "\n";

        // Place the order with risk management (using default stop-loss and take-profit)
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),order.getQuantity(),
                              order.getPrice(), /*stopLossPrice=*/0.0, /*takeProfitPrice=*/0.0,
                              marketDataFeed.getPrices());
    }

    std::cout << "\nMomentum strategy executed.\n";
}

// Function to execute the Breakout Strategy
void executeBreakoutStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    int lookbackPeriod;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;
    std::cout << "Enter the look-back period for breakout detection: ";
    std::cin >> lookbackPeriod;

    if (lookbackPeriod <= 0) {
        std::cout << "Look-back period must be a positive integer.\n";
        return;
    }

    auto strategy = std::make_unique<BreakoutStrategy>(lookbackPeriod);

    // Generate dummy historical prices simulating range-bound movement
    std::vector<double> prices(200);
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }
    prices[0] = currentPrice;
    for (size_t i = 1; i < prices.size(); ++i) {
        // Smaller increments to simulate range-bound movement
        prices[i] = prices[i - 1] + ((std::rand() % 100) - 50) / 20.0;
        if (prices[i] < 0) {
            prices[i] = 0.01;
        }
    }

    // Generate signals
    try {
        strategy->generateSignals(prices);
    } catch (const std::exception& e) {
        std::cout << "Error generating signals: " << e.what() << "\n";
        return;
    }

    // Retrieve signals and levels
    const auto& signals = strategy->getSignals();
    const auto& resistanceLevels = strategy->getResistanceLevels();
    const auto& supportLevels = strategy->getSupportLevels();

    // Display signals and levels
    std::cout << "\n=== Breakout Signals ===\n";
    for (size_t i = lookbackPeriod; i < prices.size(); ++i) {
        std::cout << "Price: $" << prices[i]
                  << ", Resistance: $" << resistanceLevels[i]
                  << ", Support: $" << supportLevels[i]
                  << ", Signal: ";
        if (signals[i] == 1) {
            std::cout << "Buy\n";
        } else if (signals[i] == -1) {
            std::cout << "Sell\n";
        } else {
            std::cout << "Neutral\n";
        }
    }

    // Generate orders
    auto orders = strategy->generateOrders(prices, symbol);

    // Display and place orders
    std::cout << "\n=== Orders Placed ===\n";
    for (const auto& order : orders) {
        std::cout << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                  << " " << order.getQuantity() << " shares of "
                  << order.getSymbol() << " at $" << order.getPrice() << "\n";

        // Place the order with risk management (using default stop-loss and take-profit)
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),order.getQuantity(),
                              order.getPrice(), /*stopLossPrice=*/0.0, /*takeProfitPrice=*/0.0,
                              marketDataFeed.getPrices());
    }

    std::cout << "\nBreakout strategy executed.\n";
}

// Function to display live prices and allow buying/selling
void viewLivePrices(TradingEngine& engine, MarketDataFeed& marketDataFeed) {
    std::cout << "\nEntering Live Price Feed mode.\n";
    std::cout << "Type 'buy' to buy, 'sell' to sell, 'view' to view your portfolio, or 'quit' to return to the main menu.\n";
    std::cout << "Press Enter to continue viewing live prices.\n";

    // Start live price updates
    marketDataFeed.startPriceUpdates();

    bool exitLiveFeed = false;

    while (!exitLiveFeed) {
        // Clear the console (optional)
        // std::system("clear"); // For Unix/Linux systems
        // std::system("cls");   // For Windows systems

        // Display live prices
        const auto& prices = marketDataFeed.getPrices();
        std::cout << "\n=== Live Market Prices ===\n";
        for (const auto& pair : prices) {
            std::cout << pair.first << ": $" << pair.second << "\n";
        }
        std::cout << "--------------------------\n";

        std::cout << "Press Enter to continue, or type a command ('buy', 'sell', 'view', 'quit'): ";
        std::string command;
        std::getline(std::cin, command);

        // Trim whitespace from command
        command.erase(0, command.find_first_not_of(" \t\n\r"));
        command.erase(command.find_last_not_of(" \t\n\r") + 1);

        if (command.empty()) {
            // User pressed Enter, continue displaying live prices
            // Sleep for 5 seconds before next price update
            std::this_thread::sleep_for(std::chrono::seconds(3));
            continue;
        }

        if (command == "buy") {
            placeOrder(engine, marketDataFeed, OrderType::Buy);
        } else if (command == "sell") {
            placeOrder(engine, marketDataFeed, OrderType::Sell);
        } else if (command == "view") {
            viewPortfolio(engine, marketDataFeed);
        } else if (command == "quit" || command == "q") {
            exitLiveFeed = true;
        } else {
            std::cout << "Invalid command. Please try again.\n";
        }
    }

//    // Stop live price updates
//    marketDataFeed.stopPriceUpdates();

    std::cout << "\nExiting Live Price Feed mode.\n";
}

// Function to view trade history
void viewTradeHistory(const TradingEngine& engine) {
    const auto& tradeHistory = engine.getPortfolio().getTradeHistory();
    if (tradeHistory.empty()) {
        std::cout << "No trade history available.\n";
    }
    else {
        std::cout << "\n--- Trade History ---\n";
        for (const auto& trade : tradeHistory) {
            const Order& order = trade.getOrder();
            std::cout << "Type: " << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                      << ", Symbol: " << order.getSymbol()
                      << ", Quantity: " << order.getQuantity()
                      << ", Price: $" << order.getPrice()
                      << ", Timestamp: " << timePointToString(order.getTimestamp())
                      << "\n";
        }
    }
}

// Function to view pending orders
void viewPendingOrders(const TradingEngine& engine) {
    const auto& pendingOrders = engine.getPortfolio().getPendingOrders();
    if (pendingOrders.empty()) {
        std::cout << "No pending orders.\n";
    }
    else {
        std::cout << "\n--- Pending Orders ---\n";
        for (const auto& order : pendingOrders) {
            std::cout << "Type: " << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                      << ", Style: " << (order.getStyle() == OrderStyle::Market ? "Market" : "Limit")
                      << ", Symbol: " << order.getSymbol()
                      << ", Quantity: " << order.getQuantity()
                      << ", Price: $" << order.getPrice()
                      << ", Stop-Loss: $" << order.getStopLoss()
                      << ", Take-Profit: $" << order.getTakeProfit()
                      << ", Timestamp: " << timePointToString(order.getTimestamp())
                      << "\n";
        }
    }
}

// Function to execute trading strategies
void executeTradingStrategies(TradingEngine& engine, MarketDataFeed& marketDataFeed) {
    std::cout << "\n--- Execute Trading Strategies ---\n";
    std::cout << "Select a strategy to execute:\n";
    std::cout << "1. Moving Average Crossover\n";
    std::cout << "2. RSI\n";
    std::cout << "3. Momentum\n";
    std::cout << "4. Breakout\n";
    std::cout << "5. Mean Reversion\n";
    std::cout << "Enter your choice: ";

    int strategyChoice = getInput<int>("");

    switch (strategyChoice) {
        case 1:
            executeMovingAverageCrossoverStrategy(engine, marketDataFeed);
            break;
        case 2:
            executeRSIStrategy(engine, marketDataFeed);
            break;
        case 3:
            executeMomentumStrategy(engine, marketDataFeed);
            break;
        case 4:
            executeBreakoutStrategy(engine, marketDataFeed);
            break;
        case 5:
            executeMeanReversionStrategy(engine, marketDataFeed);
            break;
        default:
            std::cout << "Invalid strategy selection.\n";
    }
}

// Function to update market prices (updated to process pending orders)
void updateMarketPrices(std::unordered_map<std::string, double>& marketPrices, MarketDataFeed& marketDataFeed, TradingEngine& engine) {
    std::cout << "\n--- Update Market Prices ---\n";
    std::cout << "Current Market Prices:\n";
    for (const auto& [symbol, price] : marketPrices) {
        std::cout << symbol << ": $" << price << "\n";
    }

    std::cout << "\nEnter new market prices. Type 'done' when finished.\n";
    while (true) {
        std::string inputSymbol;
        std::cout << "Enter symbol (or 'done'): ";
        std::cin >> inputSymbol;
        if (inputSymbol == "done") {
            // Clear remaining input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }

        if (marketPrices.find(inputSymbol) == marketPrices.end()) {
            std::cout << "Symbol not recognized. Please try again.\n";
            // Clear remaining input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        double newPrice = getInput<double>("Enter new price for " + inputSymbol + ": ");
        if (newPrice <= 0.0) {
            std::cout << "Price must be greater than zero. Please try again.\n";
            continue;
        }

        marketPrices[inputSymbol] = newPrice;
        marketDataFeed.updatePrice(inputSymbol, newPrice); // Update MarketDataFeed
        std::cout << "Updated " << inputSymbol << " to $" << newPrice << ".\n";
    }

    // After updating market prices, process pending orders
    engine.updateMarketData(marketPrices); // <-- Newly Modified Part
}

// Function to place a new order (modified to handle quantity)
void placeOrder(TradingEngine& engine, const MarketDataFeed& marketDataFeed, OrderType defaultType) {
    std::string symbol = getStringInput("Enter stock symbol: ");

    // Validate symbol
    auto prices = marketDataFeed.getPrices();
    if (prices.find(symbol) == prices.end()) {
        std::cout << "Symbol not found in market prices. Please update market data first.\n";
        return;
    }

    // Enter Quantity
    int quantity = getInput<int>("Enter quantity: ");
    if (quantity <= 0) {
        std::cout << "Quantity must be greater than zero.\n";
        return;
    }

    // Select Order Style: Market or Limit
    std::cout << "Select Order Style:\n1. Market\n2. Limit\nEnter your choice: ";
    int styleChoice = getInput<int>("");

    OrderStyle style;
    double entryPrice = 0.0;

    if (styleChoice == 1) {
        style = OrderStyle::Market;
        entryPrice = prices.at(symbol);
        std::cout << "Market order will be executed at current market price: $" << entryPrice << "\n";
    }
    else if (styleChoice == 2) {
        style = OrderStyle::Limit;
        entryPrice = getInput<double>("Enter limit price: ");
        if (entryPrice <= 0.0) {
            std::cout << "Limit price must be greater than zero.\n";
            return;
        }
    }
    else {
        std::cout << "Invalid Order Style selection.\n";
        return;
    }

    // Enter Stop-Loss and Take-Profit Prices
    double stopLossPrice = getInput<double>("Enter stop-loss price: ");
    double takeProfitPrice = getInput<double>("Enter take-profit price: ");

    // Place the order based on Order Type and Style
    try {
        engine.userPlaceOrder(symbol, defaultType, style, quantity, entryPrice, stopLossPrice, takeProfitPrice, prices);
    }
    catch (const std::exception& e) {
        std::cout << "Error placing order: " << e.what() << "\n";
    }
}

int main() {

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current Working Directory: " << cwd << "\n";
    } else {
        perror("getcwd() error");
    }

    // Initialize file paths for data persistence
    std::string portfolioFile = "../data/portfolio.json";
    std::string ordersFile = "../data/orders.json";
    std::string plFile = "../data/pl.json"; // Newly added
    // Example: Using Absolute Path (Adjust accordingly)
    // std::string plFile = "/Users/jack/Desktop/project/clion/cpp/project01/data/pl.json";

    // Initialize DataPersistence
    DataPersistence dataPersistence(portfolioFile, ordersFile, plFile); // newly added

    // Initialize random number generator
    initializeRandomGenerator();

    // List of stock symbols to track
    std::vector<std::string> symbols = {"AAPL", "GOOGL", "MSFT", "AMZN"};

    // Initialize the market data feed with initial prices
    std::unordered_map<std::string, double> initialPrices = {
            {"AAPL",  150.0},
            {"GOOGL", 280.0},
            {"MSFT",  300.0},
            {"AMZN",  350.0}
    };

    // Initialize MarketDataFeed with symbols and initial prices
    MarketDataFeed marketDataFeed(symbols, initialPrices);

    // Initialize TradingEngine with initial capital (e.g., $100,000)
    double initialCapital = 1000000.0;
    TradingEngine engine(initialCapital);

    // MarketDataFeed marketDataFeed;

    // Load existing portfolio if available
    Portfolio loadedPortfolio(initialCapital);
    if (dataPersistence.loadPortfolio(loadedPortfolio)) {
        engine.setPortfolio(loadedPortfolio);
        std::cout << "Portfolio loaded successfully from " << portfolioFile << ".\n";
    } else {
        std::cout << "Portfolio file not found or corrupted. Starting with a new portfolio.\n";
    }

    // Load existing orders if available
    std::vector<Order> loadedOrders;
    if (dataPersistence.loadOrders(loadedOrders)) {
        std::cout << "Orders loaded successfully from " << ordersFile << ".\n";
        // Add loaded orders to the portfolio's pending orders
        for (const auto &order: loadedOrders) {
            engine.getPortfolio().addPendingOrder(order);
        }
    } else {
        std::cout << "Orders file not found or corrupted. Starting with no pending orders.\n";
    }

    // Load existing Profit & Loss records if available
    std::vector<ProfitLoss> loadedPL;
    if (dataPersistence.loadProfitLoss(loadedPL)) {
        engine.getPortfolio().setProfitLossList(loadedPL);
        std::cout << "Profit & Loss records loaded successfully from " << plFile << ".\n";
    }
    else {
        std::cout << "Starting with no Profit & Loss records.\n";
    }


    // Initialize market prices before placing loaded orders
    std::unordered_map<std::string, double> marketPrices = marketDataFeed.getPrices();

    int choice;
    do {
        displayMainMenu();
        std::cin >> choice;
        switch (choice) {
            case 1: { // Place a New Order
                std::cout << "\nSelect Order Type:\n1. Buy\n2. Sell\nEnter your choice: ";
                int orderTypeChoice = getInput<int>("");
                if (orderTypeChoice == 1) {
                    placeOrder(engine, marketDataFeed, OrderType::Buy);
                } else if (orderTypeChoice == 2) {
                    placeOrder(engine, marketDataFeed, OrderType::Sell);
                } else {
                    std::cout << "Invalid Order Type selection.\n";
                }
                break;
            }
            case 2: { // View Portfolio
                viewPortfolio(engine, marketDataFeed);
                break;
            }
            case 3: { // View Trade History
                viewTradeHistory(engine);
                break;
            }
            case 4: { // View Pending Orders
                viewPendingOrders(engine);
                break;
            }
            case 5: { // Update Market Prices
                updateMarketPrices(marketPrices, marketDataFeed, engine);
                break;
            }
            case 6: { // View Live Prices
                viewLivePrices(engine, marketDataFeed);
                break;
            }
            case 7: { // View Market Prices
                viewMarketPrices(marketDataFeed);
                break;
            }
            case 8: { // Execute Trading Strategies
                executeTradingStrategies(engine, marketDataFeed);
                break;
            }
            case 9: { // Save and Exit
                // Save Portfolio
                if (dataPersistence.savePortfolio(engine.getPortfolio())) {
                    std::cout << "Portfolio saved successfully to " << portfolioFile << ".\n";
                }
                else {
                    std::cout << "Failed to save portfolio to " << portfolioFile << ".\n";
                }

                // Save Orders
                std::vector<Order> allOrders = engine.getAllOrders();
                if (dataPersistence.saveOrders(allOrders)) {
                    std::cout << "Orders saved successfully to " << ordersFile << ".\n";
                }
                else {
                    std::cout << "Failed to save orders to " << ordersFile << ".\n";
                }

                // Save Profit & Loss
                const auto& plList = engine.getPortfolio().getProfitLossList();
                if (dataPersistence.saveProfitLoss(plList)) {
                    std::cout << "Profit & Loss records saved successfully to " << plFile << ".\n";
                }
                else {
                    std::cout << "Failed to save Profit & Loss records to " << plFile << ".\n";
                }

                std::cout << "Exiting the program.\n";
                break;
            }
            case 0:
                std::cout << "Exiting application.\n";
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }

    } while (choice != 0);

        return 0;
}
