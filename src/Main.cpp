//#include <iostream>
//#include "MovingAverageCrossoverStrategy.h"
//#include "RSIStrategy.h" // Assuming you have implemented this strategy
//#include "StrategyManager.h"
//#include "TradingEngine.h"
//#include <unordered_map>
//#include "MarketDataFeed.h"
//
//
//void viewPortfolio(const TradingEngine& engine, const MarketDataFeed& marketDataFeed);
//void placeOrder(TradingEngine& engine, MarketDataFeed& marketDataFeed);
//void viewMarketPrices(const MarketDataFeed& marketDataFeed);
//void displayMainMenu();
//
//int main() {
//
//    // Initialize the trading engine with $10,000 cash :  2024 Oct 9
//    TradingEngine engine(10000.0);
//
//    // Initialize the market data feed
//    MarketDataFeed marketDataFeed;
//
//    int choice;
//    do {
//        displayMainMenu();
//        std::cin >> choice;
//
//        switch (choice) {
//            case 1:
//                viewMarketPrices(marketDataFeed);
//                break;
//            case 2:
//                placeOrder(engine, marketDataFeed);
//                break;
//            case 3:
//                viewPortfolio(engine, marketDataFeed);
//                break;
//            case 4:
//                marketDataFeed.updatePrices();
//                // Process pending limit orders
//                engine.processPendingOrders(marketDataFeed.getPrices());
//                std::cout << "Market prices updated.\n";
//                break;
//            case 0:
//                std::cout << "Exiting application.\n";
//                break;
//            default:
//                std::cout << "Invalid choice. Please try again.\n";
//                break;
//        }
//    } while (choice != 0);
//
//    // Set a trading strategy : 2024 Oct 9
//    engine.setStrategy(std::make_unique<MovingAverageCrossoverStrategy>(3, 5));
//
//
//    // Sample price data (e.g., cryptocurrency closing prices)
//    std::vector<double> prices = {
//            100.0, 101.5, 102.3, 103.7, 104.1,
//            103.5, 102.8, 101.2, 100.5, 99.8,
//            100.2, 101.0, 102.5, 103.0, 104.5
//    };
//
//    std::string symbol = "AAPL"; // Example symbol : 2024 Oct 9
//
//    // Execute strategy: 2024 Oct 9
//    engine.executeStrategy(prices, symbol);
//
//    // User interaction: Sell 30 shares at $110: 2024 Oct 9
//    engine.userPlaceOrder(symbol, OrderType::Sell, OrderStyle::Market, 30, 110.0, marketDataFeed.getPrices());
//
//
//    // Display portfolio status: 2024 Oct 9
//    std::unordered_map<std::string, double> marketPrices = { { symbol, 108.0 } };
//    std::cout << "Cash Balance: $" << engine.getCashBalance() << "\n";
//    std::cout << "Position in " << symbol << ": " << engine.getPosition(symbol) << " shares\n";
//    std::cout << "Unrealized P&L: $" << engine.getUnrealizedPnL(marketPrices) << "\n";
//
//
//
//    // Create a StrategyManager instance
//    StrategyManager manager;
//
//    // **Option 1: Using a Single Strategy**
//
//    // Instantiate a Moving Average Crossover Strategy
//    auto macStrategy = std::make_unique<MovingAverageCrossoverStrategy>(3, 5);
//
//    // Set the strategy in the manager
//    manager.setStrategy(std::move(macStrategy));
//
//    // Execute the strategy
//    manager.executeStrategy(prices);
//
//    // Retrieve and display the signals
//    const std::vector<int>& signals = manager.getSignals();
//
//    std::cout << "Single Strategy (Moving Average Crossover) Signals:\n";
//    std::cout << "Date\tPrice\tSignal\n";
//    for (size_t i = 0; i < prices.size(); ++i) {
//        std::string signalText;
//        if (signals[i] == 1) {
//            signalText = "Buy";
//        } else if (signals[i] == -1) {
//            signalText = "Sell";
//        } else {
//            signalText = "Hold";
//        }
//        std::cout << i + 1 << "\t" << prices[i] << "\t" << signalText << "\n";
//    }
//
//    // **Option 2: Using Multiple Strategies**
//
//    // Clear the previous strategy
//    manager.clearStrategies();
//
//    // Add multiple strategies
//    manager.addStrategy(std::make_unique<MovingAverageCrossoverStrategy>(3, 5));
//    manager.addStrategy(std::make_unique<RSIStrategy>(14)); // Example RSI strategy
//
//    // Execute all strategies
//    manager.executeStrategy(prices);
//
//    // Retrieve and display the signals from all strategies
//    const auto& allSignals = manager.getAllSignals();
//
//    std::cout << "\nMultiple Strategies Signals:\n";
//
//    for (size_t stratIndex = 0; stratIndex < allSignals.size(); ++stratIndex) {
//        std::cout << "Strategy " << stratIndex + 1 << " Signals:\n";
//        std::cout << "Date\tPrice\tSignal\n";
//        for (size_t i = 0; i < prices.size(); ++i) {
//            std::string signalText;
//            int signal = allSignals[stratIndex][i];
//            if (signal == 1) {
//                signalText = "Buy";
//            } else if (signal == -1) {
//                signalText = "Sell";
//            } else {
//                signalText = "Hold";
//            }
//            std::cout << i + 1 << "\t" << prices[i] << "\t" << signalText << "\n";
//        }
//        std::cout << "\n";
//    }
//
//    return 0;
//}
//
//
//
//// main.cpp
////
////#include "TradingEngine.h"
////#include "MarketDataFeed.h"
////// Include other necessary headers
////#include <iostream>
////
////int main() {
////    // Initialize the trading engine with $10,000 cash
////    TradingEngine engine(10000.0);
////
////    // Initialize the market data feed
////    MarketDataFeed marketDataFeed;
////
////    // Main application loop
////    int choice;
////    do {
////        displayMainMenu();
////        std::cin >> choice;
////
////        switch (choice) {
////            case 1:
////                viewMarketPrices(marketDataFeed);
////                break;
////            case 2:
////                placeOrder(engine, marketDataFeed);
////                break;
////            case 3:
////                viewPortfolio(engine, marketDataFeed);
////                break;
////            case 4:
////                marketDataFeed.updatePrices();
////                engine.processPendingOrders(marketDataFeed.getPrices());
////                std::cout << "Market prices updated.\n";
////                break;
////            case 0:
////                std::cout << "Exiting application.\n";
////                break;
////            default:
////                std::cout << "Invalid choice. Please try again.\n";
////                break;
////        }
////    } while (choice != 0);
////
////    return 0;
////}
//// Implement functions to display menus and handle user choices: 2024 Oct 9a
//void displayMainMenu() {
//    std::cout << "\n=== Trading Application Menu ===\n";
//    std::cout << "1. View Market Prices\n";
//    std::cout << "2. Place Order\n";
//    std::cout << "3. View Portfolio\n";
//    std::cout << "4. Update Market Prices\n";
//    std::cout << "0. Exit\n";
//    std::cout << "Enter your choice: ";
//}
//
//void displayOrderMenu() {
//    std::cout << "\n=== Place Order ===\n";
//    std::cout << "Enter the following details:\n";
//}
//
//
//// Add functions to handle user actions: 2024 Oct 9a
//
//
//void viewMarketPrices(const MarketDataFeed& marketDataFeed) {
//    std::cout << "\n=== Market Prices ===\n";
//    const auto& prices = marketDataFeed.getPrices();
//    for (const auto& pair : prices) {
//        std::cout << pair.first << ": $" << pair.second << "\n";
//    }
//}
//
//void placeOrder(TradingEngine& engine, MarketDataFeed& marketDataFeed) {
//    std::string symbol;
//    std::string orderTypeStr;
//    std::string orderStyleStr;
//    int quantity;
//    double price;
//
//    displayOrderMenu();
//    std::cout << "Stock Symbol (e.g., AAPL): ";
//    std::cin >> symbol;
//
//    std::cout << "Order Type (Buy/Sell): ";
//    std::cin >> orderTypeStr;
//    OrderType orderType;
//    if (orderTypeStr == "Buy" || orderTypeStr == "buy") {
//        orderType = OrderType::Buy;
//    } else if (orderTypeStr == "Sell" || orderTypeStr == "sell") {
//        orderType = OrderType::Sell;
//    } else {
//        std::cout << "Invalid order type.\n";
//        return;
//    }
//
//    std::cout << "Order Style (Market/Limit): ";
//    std::cin >> orderStyleStr;
//    OrderStyle orderStyle;
//    if (orderStyleStr == "Market" || orderStyleStr == "market") {
//        orderStyle = OrderStyle::Market;
//    } else if (orderStyleStr == "Limit" || orderStyleStr == "limit") {
//        orderStyle = OrderStyle::Limit;
//    } else {
//        std::cout << "Invalid order style.\n";
//        return;
//    }
//
//    std::cout << "Quantity: ";
//    std::cin >> quantity;
//
//    if (orderStyle == OrderStyle::Limit) {
//        std::cout << "Limit Price: ";
//        std::cin >> price;
//    } else {
//        // For market orders, use current market price
//        try {
//            price = marketDataFeed.getPrice(symbol);
//        } catch (const std::exception& e) {
//            std::cout << e.what() << "\n";
//            return;
//        }
//    }
//
//    engine.userPlaceOrder(symbol, orderType, orderStyle, quantity, price, marketDataFeed.getPrices());
//}
//
//void viewPortfolio(const TradingEngine& engine, const MarketDataFeed& marketDataFeed) {
//    std::cout << "\n=== Portfolio ===\n";
//    std::cout << "Cash Balance: $" << engine.getCashBalance() << "\n";
//
//    const auto& positions = engine.getPositions();
//    if (positions.empty()) {
//        std::cout << "No positions.\n";
//    } else {
//        std::cout << "Positions:\n";
//        for (const auto& pair : positions) {
//            std::cout << pair.first << ": " << pair.second << " shares\n";
//        }
//    }
//
//    double unrealizedPnL = engine.getUnrealizedPnL(marketDataFeed.getPrices());
//    std::cout << "Unrealized P&L: $" << unrealizedPnL << "\n";
//}



// main.cpp

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdlib>  // For std::rand(), std::srand()
#include <ctime>    // For std::time()

// Include all the necessary headers
#include "TradingEngine.h"
#include "MarketDataFeed.h"
#include "Order.h"
#include "Portfolio.h"
#include "TradingStrategy.h"
#include "MovingAverageCrossoverStrategy.h"
#include "Trade.h"

// Function declarations
void displayMainMenu();
void displayOrderMenu();
void viewMarketPrices(const MarketDataFeed& marketDataFeed);
void placeOrder(TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void viewPortfolio(const TradingEngine& engine, const MarketDataFeed& marketDataFeed);
void executeTradingStrategy(TradingEngine& engine, const MarketDataFeed& marketDataFeed);

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
    std::cout << "5. Run Trading Strategy\n";
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