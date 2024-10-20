// main.cpp

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdlib>  // For std::rand(), std::srand()
#include <ctime>    // For std::time()
#include <stdexcept> // For std::exception

// Include all the necessary headers
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

// Function declarations
void displayMainMenu();
void displayOrderMenu();
void viewMarketPrices(const MarketDataFeed& marketDataFeed);
void placeOrder(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void viewPortfolio(const TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeTradingStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeRSIStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeMomentumStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeBreakoutStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeMeanReversionStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);

int main() {
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initialize the trading engine with $10,000 cash
    TradingEngine engine(10000.0);

    // Initialize the market data feed
    MarketDataFeed marketDataFeed;

    int choice;
    do {
        displayMainMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                viewMarketPrices(marketDataFeed);
                break;
            case 2:
                placeOrder(engine, marketDataFeed);
                break;
            case 3:
                viewPortfolio(engine, marketDataFeed);
                break;
            case 4:
                marketDataFeed.updatePrices();
                engine.processPendingOrders(marketDataFeed.getPrices());
                std::cout << "Market prices updated.\n";
                break;
            case 5:
                executeTradingStrategy(engine, marketDataFeed);
                break;
            case 6:
                executeRSIStrategy(engine, marketDataFeed);
                break;
            case 7:
                executeMomentumStrategy(engine, marketDataFeed);
                break;
            case 8:
                executeBreakoutStrategy(engine, marketDataFeed);
                break;
            case 9:
                executeMeanReversionStrategy(engine, marketDataFeed);
                break;
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

void displayMainMenu() {
    std::cout << "\n=== Trading Application Menu ===\n";
    std::cout << "1. View Market Prices\n";
    std::cout << "2. Place Order\n";
    std::cout << "3. View Portfolio\n";
    std::cout << "4. Update Market Prices\n";
    std::cout << "5. Run Moving Average Crossover Strategy\n";
    std::cout << "6. Run RSI Strategy\n";
    std::cout << "7. Run Momentum Strategy\n";
    std::cout << "8. Run Breakout Strategy\n";
    std::cout << "9. Run Mean Reversion Strategy\n"; // New option
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

void displayOrderMenu() {
    std::cout << "\n=== Place Order ===\n";
    std::cout << "Enter the following details:\n";
}

void viewMarketPrices(const MarketDataFeed& marketDataFeed) {
    std::cout << "\n=== Market Prices ===\n";
    const auto& prices = marketDataFeed.getPrices();
    for (const auto& pair : prices) {
        std::cout << pair.first << ": $" << pair.second << "\n";
    }
}

void placeOrder(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    std::string orderTypeStr;
    std::string orderStyleStr;
    int quantity;
    double price;

    displayOrderMenu();
    std::cout << "Stock Symbol (e.g., AAPL): ";
    std::cin >> symbol;

    std::cout << "Order Type (Buy/Sell): ";
    std::cin >> orderTypeStr;
    OrderType orderType;
    if (orderTypeStr == "Buy" || orderTypeStr == "buy") {
        orderType = OrderType::Buy;
    } else if (orderTypeStr == "Sell" || orderTypeStr == "sell") {
        orderType = OrderType::Sell;
    } else {
        std::cout << "Invalid order type.\n";
        return;
    }

    std::cout << "Order Style (Market/Limit): ";
    std::cin >> orderStyleStr;
    OrderStyle orderStyle;
    if (orderStyleStr == "Market" || orderStyleStr == "market") {
        orderStyle = OrderStyle::Market;
    } else if (orderStyleStr == "Limit" || orderStyleStr == "limit") {
        orderStyle = OrderStyle::Limit;
    } else {
        std::cout << "Invalid order style.\n";
        return;
    }

    std::cout << "Quantity: ";
    std::cin >> quantity;

    if (orderStyle == OrderStyle::Limit) {
        std::cout << "Limit Price: ";
        std::cin >> price;
    } else {
        // For market orders, use current market price
        try {
            price = marketDataFeed.getPrice(symbol);
        } catch (const std::exception& e) {
            std::cout << e.what() << "\n";
            return;
        }
    }

    // Now place the order
    engine.userPlaceOrder(symbol, orderType, orderStyle, quantity, price, marketDataFeed.getPrices());
}

void viewPortfolio(const TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::cout << "\n=== Portfolio ===\n";
    std::cout << "Cash Balance: $" << engine.getCashBalance() << "\n";

    const auto& positions = engine.getPositions();
    if (positions.empty()) {
        std::cout << "No positions.\n";
    } else {
        std::cout << "Positions:\n";
        for (const auto& pair : positions) {
            std::cout << pair.first << ": " << pair.second << " shares\n";
        }
    }

    double unrealizedPnL = engine.getUnrealizedPnL(marketDataFeed.getPrices());
    std::cout << "Unrealized P&L: $" << unrealizedPnL << "\n";
}

void executeTradingStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    // For demonstration, we can create a MovingAverageCrossoverStrategy and execute it
    std::string symbol;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;

    int shortWindow, longWindow;
    std::cout << "Enter the short moving average window size: ";
    std::cin >> shortWindow;
    std::cout << "Enter the long moving average window size: ";
    std::cin >> longWindow;

    if (shortWindow >= longWindow) {
        std::cout << "Short window must be less than long window.\n";
        return;
    }

    // Create the strategy
    auto strategy = std::make_unique<MovingAverageCrossoverStrategy>(shortWindow, longWindow);

    // Generate historical prices for the symbol
    std::vector<double> prices;
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    // Generate dummy historical prices around the current price
    prices.resize(200);
    for (size_t i = 0; i < prices.size(); ++i) {
        prices[i] = currentPrice + ((std::rand() % 200) - 100) / 10.0;
    }

    // Execute the strategy
    try {
        strategy->generateSignals(prices);
    } catch (const std::exception& e) {
        std::cout << "Error generating signals: " << e.what() << "\n";
        return;
    }
    auto orders = strategy->generateOrders(prices, symbol);

    // Place the orders
    for (const auto& order : orders) {
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(), order.getQuantity(), order.getPrice(), marketDataFeed.getPrices());
    }

    std::cout << "Trading strategy executed.\n";
}

// Add a function to execute the RSI strategy.
// This function will be similar to the executeTradingStrategy function,
// but will use the RSIStrategy class instead of the MovingAverageCrossoverStrategy class.
// 2024 Oct 10

void executeRSIStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;

    int period;
    std::cout << "Enter the RSI period (e.g., 14): ";
    std::cin >> period;

    double overboughtThreshold, oversoldThreshold;
    std::cout << "Enter the overbought threshold (default 70): ";
    std::cin >> overboughtThreshold;
    std::cout << "Enter the oversold threshold (default 30): ";
    std::cin >> oversoldThreshold;

    if (overboughtThreshold <= oversoldThreshold) {
        std::cout << "Overbought threshold must be greater than oversold threshold.\n";
        return;
    }

    auto strategy = std::make_unique<RSIStrategy>(period, overboughtThreshold, oversoldThreshold);

    std::vector<double> prices;
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    // Generate dummy historical prices around the current price
    prices.resize(200);
    for (size_t i = 0; i < prices.size(); ++i) {
        prices[i] = currentPrice + ((std::rand() % 200) - 100) / 10.0;
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
    std::cout << "\n=== RSI Values and Signals ===\n";
    for (size_t i = period + 1; i < prices.size(); ++i) {
        std::cout << "Price: $" << prices[i]
                  << ", RSI: " << rsiValues[i]
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

        // Place the order
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
                              order.getQuantity(), order.getPrice(), marketDataFeed.getPrices());
    }

    std::cout << "\nRSI strategy executed.\n";
}



// Add a function to execute the Momentum strategy. This function will be similar to the executeTradingStrategy function
// but will use the MomentumStrategy class instead of the MovingAverageCrossoverStrategy class. 2024 Oct 10

void executeMomentumStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;

    int lookbackPeriod;
    std::cout << "Enter the look-back period for momentum calculation: ";
    std::cin >> lookbackPeriod;

    double buyThreshold, sellThreshold;
    std::cout << "Enter the buy threshold (e.g., 0.05 for 5%): ";
    std::cin >> buyThreshold;
    std::cout << "Enter the sell threshold (e.g., -0.05 for -5%): ";
    std::cin >> sellThreshold;

    if (buyThreshold <= sellThreshold) {
        std::cout << "Buy threshold must be greater than sell threshold.\n";
        return;
    }

    auto strategy = std::make_unique<MomentumStrategy>(lookbackPeriod, buyThreshold, sellThreshold);

    std::vector<double> prices;
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    // Generate dummy historical prices around the current price
    prices.resize(200);
    prices[0] = currentPrice;
    for (size_t i = 1; i < prices.size(); ++i) {
        prices[i] = prices[i - 1] + ((std::rand() % 200) - 100) / 10.0;
    }

    // Generate signals
    try {
        strategy->generateSignals(prices);
    } catch (const std::exception& e) {
        std::cout << "Error generating signals: " << e.what() << "\n";
        return;
    }

    // Retrieve momentum values and signals
    const auto& momentumValues = strategy->getMomentumValues();
    const auto& signals = strategy->getSignals();

    // Display momentum values and signals
    std::cout << "\n=== Momentum Values and Signals ===\n";
    for (size_t i = lookbackPeriod; i < prices.size(); ++i) {
        std::cout << "Price: $" << prices[i]
                  << ", Momentum: " << momentumValues[i]
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

        // Place the order
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
                              order.getQuantity(), order.getPrice(), marketDataFeed.getPrices());
    }

    std::cout << "\nMomentum strategy executed.\n";
}

// add a function to execute the Breakout strategy.
// This function will be similar to the executeTradingStrategy function

void executeBreakoutStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;

    int lookbackPeriod;
    std::cout << "Enter the look-back period for breakout detection: ";
    std::cin >> lookbackPeriod;

    if (lookbackPeriod <= 0) {
        std::cout << "Look-back period must be a positive integer.\n";
        return;
    }

    auto strategy = std::make_unique<BreakoutStrategy>(lookbackPeriod);

    std::vector<double> prices;
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    // Generate dummy historical prices around the current price
    prices.resize(200);
    prices[0] = currentPrice;
    for (size_t i = 1; i < prices.size(); ++i) {
        prices[i] = prices[i - 1] + ((std::rand() % 100) - 50) / 10.0; // Smaller increments to simulate range-bound movement
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

        // Place the order
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
                              order.getQuantity(), order.getPrice(), marketDataFeed.getPrices());
    }

    std::cout << "\nBreakout strategy executed.\n";
}


void executeMeanReversionStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    std::cout << "Enter the stock symbol for the strategy (e.g., AAPL): ";
    std::cin >> symbol;

    int lookbackPeriod;
    std::cout << "Enter the look-back period for mean calculation: ";
    std::cin >> lookbackPeriod;

    double deviationThreshold;
    std::cout << "Enter the deviation threshold (e.g., 0.05 for 5%): ";
    std::cin >> deviationThreshold;

    if (lookbackPeriod <= 0 || deviationThreshold <= 0) {
        std::cout << "Look-back period and deviation threshold must be positive numbers.\n";
        return;
    }

    auto strategy = std::make_unique<MeanReversionStrategy>(lookbackPeriod, deviationThreshold);

    std::vector<double> prices;
    double currentPrice;
    try {
        currentPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    // Generate dummy historical prices simulating mean reversion
    prices.resize(200);
    prices[0] = currentPrice;
    for (size_t i = 1; i < prices.size(); ++i) {
        double randomNoise = ((std::rand() % 200) - 100) / 50.0; // Random noise
        prices[i] = prices[i - 1] + randomNoise;
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

    // Generate orders
    auto orders = strategy->generateOrders(prices, symbol);

    // Display and place orders
    std::cout << "\n=== Orders Placed ===\n";
    for (const auto& order : orders) {
        std::cout << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                  << " " << order.getQuantity() << " shares of "
                  << order.getSymbol() << " at $" << order.getPrice() << "\n";

        // Place the order
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
                              order.getQuantity(), order.getPrice(), marketDataFeed.getPrices());
    }

    std::cout << "\nMean Reversion strategy executed.\n";
}