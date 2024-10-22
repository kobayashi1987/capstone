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


// Seed the random number generator
void initializeRandomGenerator() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

// Function to display the main menu
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
    std::cout << "9. Run Mean Reversion Strategy\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

// Function to view current market prices
void viewMarketPrices(const MarketDataFeed& marketDataFeed) {
    const auto& prices = marketDataFeed.getPrices();
    std::cout << "\n=== Current Market Prices ===\n";
    for (const auto& pair : prices) {
        std::cout << pair.first << ": $" << pair.second << "\n";
    }
}

// Function to place an order with risk management features
void placeOrder(TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::string symbol;
    std::cout << "Enter the stock symbol: ";
    std::cin >> symbol;

    std::string orderTypeStr;
    std::cout << "Order Type (Buy/Sell): ";
    std::cin >> orderTypeStr;
    OrderType orderType = (orderTypeStr == "Buy") ? OrderType::Buy : OrderType::Sell;

    OrderStyle orderStyle = OrderStyle::Market; // Assuming market orders for simplicity

    double stopLossPrice = 0.0;
    double takeProfitPrice = 0.0;

    std::cout << "Stop-Loss Price (enter 0 for no stop-loss): ";
    std::cin >> stopLossPrice;

    std::cout << "Take-Profit Price (enter 0 for no take-profit): ";
    std::cin >> takeProfitPrice;

    double entryPrice;
    try {
        entryPrice = marketDataFeed.getPrice(symbol);
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
        return;
    }

    // Place the order with risk management
    try {
        engine.userPlaceOrder(symbol, orderType, orderStyle, entryPrice, stopLossPrice, takeProfitPrice, marketDataFeed.getPrices());
    } catch (const std::exception& e) {
        std::cout << "Error placing order: " << e.what() << "\n";
    }
}

// Function to view the current portfolio
void viewPortfolio(const TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
    std::cout << "\n=== Portfolio ===\n";
    std::cout << "Cash Balance: $" << engine.getCashBalance() << "\n";

    // Declare 'marketPrices' here to make it available throughout the function
    const auto& marketPrices = marketDataFeed.getPrices();

    const auto& positions = engine.getPositions();
    if (positions.empty()) {
        std::cout << "No open positions.\n";
    } else {
        for (const auto& pair : positions) {
            const std::string& symbol = pair.first;
            const Position& position = pair.second;
            double marketPrice = marketPrices.at(symbol);
            std::cout << symbol << ": " << position.quantity << " shares at average price $" << position.averagePrice
                      << ", Current Price: $" << marketPrice << "\n";
        }
    }

    double totalPortfolioValue = engine.getTotalPortfolioValue(marketPrices);
    std::cout << "Total Portfolio Value: $" << totalPortfolioValue << "\n";
}

// Function to update market prices (simulated for testing)
void updateMarketPrices(MarketDataFeed& marketDataFeed) {
    // Simulate price updates by adding random fluctuations
    auto& prices = marketDataFeed.getPricesMutable();
    for (auto& pair : prices) {
        double fluctuation = ((std::rand() % 200) - 100) / 100.0; // Random fluctuation between -1 and +1
        pair.second += fluctuation;
        if (pair.second < 0) {
            pair.second = 0.01; // Ensure price doesn't go negative
        }
    }
    std::cout << "Market prices updated.\n";
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
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
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
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
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
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
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
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
                              order.getPrice(), /*stopLossPrice=*/0.0, /*takeProfitPrice=*/0.0,
                              marketDataFeed.getPrices());
    }

    std::cout << "\nBreakout strategy executed.\n";
}

// Function to execute the Mean Reversion Strategy
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
        double randomNoise = ((std::rand() % 200) - 100) / 50.0; // Random noise
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

    // Generate orders
    auto orders = strategy->generateOrders(prices, symbol);

    // Display and place orders
    std::cout << "\n=== Orders Placed ===\n";
    for (const auto& order : orders) {
        std::cout << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                  << " " << order.getQuantity() << " shares of "
                  << order.getSymbol() << " at $" << order.getPrice() << "\n";

        // Place the order with risk management (using default stop-loss and take-profit)
        engine.userPlaceOrder(order.getSymbol(), order.getType(), order.getStyle(),
                              order.getPrice(), /*stopLossPrice=*/0.0, /*takeProfitPrice=*/0.0,
                              marketDataFeed.getPrices());
    }

    std::cout << "\nMean Reversion strategy executed.\n";
}

int main() {
    // Initialize random number generator
    initializeRandomGenerator();

    // List of stock symbols to track
    std::vector<std::string> symbols = {"AAPL", "GOOGL", "MSFT", "AMZN"};

    // Initialize the market data feed with initial prices
    std::unordered_map<std::string, double> initialPrices = {
            {"AAPL", 150.0},
            {"GOOGL", 2800.0},
            {"MSFT", 300.0},
            {"AMZN", 3500.0}
    };
    MarketDataFeed marketDataFeed(symbols, initialPrices);

    // Initialize the trading engine with $10,000 cash
    TradingEngine engine(10000.0);

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
                updateMarketPrices(marketDataFeed);
                // Update market data in the trading engine
                engine.updateMarketData(marketDataFeed.getPrices());
                break;
            case 5:
                executeMovingAverageCrossoverStrategy(engine, marketDataFeed);
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