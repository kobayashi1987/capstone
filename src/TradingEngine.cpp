////
//// Created by jack on 9/10/24.
////
//
//
//#include "TradingEngine.h"
//#include "Order.h"
//#include "Trade.h"
//#include "MarketDataFeed.h"
//#include "Portfolio.h"
//#include <iostream>
//#include <cmath>
//#include <stdexcept>
//
//// Constructor: Initializes the TradingEngine with the initial capital
//TradingEngine::TradingEngine(double initialCapital)
//        : portfolio_(initialCapital) {
//    // Initialize peak portfolio value with initial capital
//    portfolio_.setPeakValue(initialCapital);
//}
//
//// Method to calculate position size based on risk per trade
//int TradingEngine::calculatePositionSize(double entryPrice, double stopLossPrice, double riskPerTrade) {
//    // Calculate the risk per share
//    double riskPerShare = std::abs(entryPrice - stopLossPrice);
//    if (riskPerShare <= 0) {
//        throw std::invalid_argument("Risk per share must be greater than zero.");
//    }
//    // Calculate position size by dividing risk per trade by risk per share
//    int positionSize = static_cast<int>(riskPerTrade / riskPerShare);
//    return positionSize;
//}
//
//// Updated method to place orders with risk management
//void TradingEngine::userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style,
//                                   double entryPrice, double stopLossPrice, double takeProfitPrice,
//                                   const std::unordered_map<std::string, double>& marketPrices) {
//    // Check maximum drawdown
//    double maxDrawdownLimit = 0.2; // 20% maximum drawdown
//    if (portfolio_.getDrawdown() >= maxDrawdownLimit) {
//        std::cout << "Maximum drawdown limit reached (" << maxDrawdownLimit * 100 << "%). Cannot place new orders.\n";
//        return;
//    }
//
//    // Determine risk per trade (e.g., 1% of total capital)
//    double riskPercentage = 0.01; // 1% risk per trade
//    double totalCapital = portfolio_.getCashBalance() + portfolio_.getPortfolioValue(marketPrices);
//    double riskPerTrade = totalCapital * riskPercentage;
//
//    // Calculate potential loss and profit per share
//    double potentialLoss = std::abs(entryPrice - stopLossPrice);
//    double potentialProfit = std::abs(takeProfitPrice - entryPrice);
//
//    // Check for valid stop-loss and take-profit prices
//    if (stopLossPrice <= 0 || takeProfitPrice <= 0 || potentialLoss == 0 || potentialProfit == 0) {
//        std::cout << "Invalid stop-loss or take-profit prices.\n";
//        return;
//    }
//
//    // Calculate risk-reward ratio
//    double riskRewardRatio = potentialProfit / potentialLoss;
//
//    // Minimum acceptable risk-reward ratio (e.g., 2:1)
//    double minimumRiskRewardRatio = 2.0;
//
//    // Check if the risk-reward ratio meets the minimum acceptable threshold
//    if (riskRewardRatio < minimumRiskRewardRatio) {
//        std::cout << "Risk-reward ratio is below the acceptable threshold (" << minimumRiskRewardRatio << ").\n";
//        return;
//    }
//
//    // Calculate position size based on risk per trade
//    int positionSize;
//    try {
//        positionSize = calculatePositionSize(entryPrice, stopLossPrice, riskPerTrade);
//    } catch (const std::exception& e) {
//        std::cout << "Error calculating position size: " << e.what() << "\n";
//        return;
//    }
//
//    // Check maximum allocation per asset (e.g., 20% of total capital)
//    double maxAllocationPercentage = 0.2; // 20%
//    double maxAllocationValue = totalCapital * maxAllocationPercentage;
//    double currentPositionValue = portfolio_.getPositionValue(symbol, marketPrices);
//    double newPositionValue = currentPositionValue + (positionSize * entryPrice);
//
//    // If new position exceeds maximum allocation, adjust the position size
//    if (newPositionValue > maxAllocationValue) {
//        // Adjust position size to not exceed maximum allocation
//        int adjustedPositionSize = static_cast<int>((maxAllocationValue - currentPositionValue) / entryPrice);
//        if (adjustedPositionSize <= 0) {
//            std::cout << "Position size exceeds maximum allocation per asset (" << maxAllocationPercentage * 100
//                      << "%). Cannot place order.\n";
//            return;
//        } else {
//            std::cout << "Position size exceeds maximum allocation per asset (" << maxAllocationPercentage * 100
//                      << "%). Adjusting position size to " << adjustedPositionSize << " shares.\n";
//            positionSize = adjustedPositionSize;
//        }
//    }
//
//    // Check if cash balance is sufficient for the trade
//    double requiredCash = positionSize * entryPrice;
//    if (requiredCash > portfolio_.getCashBalance()) {
//        std::cout << "Insufficient cash balance to place the order.\n";
//        return;
//    }
//
//    // Create and place the order with stop-loss and take-profit prices
//    Order order(type, style, symbol, positionSize, entryPrice, stopLossPrice, takeProfitPrice);
//    portfolio_.placeOrder(order, marketPrices);
//
//    // Output order details
//    std::cout << "Order placed: " << (type == OrderType::Buy ? "Buy" : "Sell") << " "
//              << positionSize << " shares of " << symbol << " at $" << entryPrice << "\n";
//    std::cout << "Risk-Reward Ratio: " << riskRewardRatio << "\n";
//    if (stopLossPrice > 0.0) {
//        std::cout << "Stop-Loss Price: $" << stopLossPrice << "\n";
//    }
//    if (takeProfitPrice > 0.0) {
//        std::cout << "Take-Profit Price: $" << takeProfitPrice << "\n";
//    }
//}
//
//// Method to update market data and process stop orders
//void TradingEngine::updateMarketData(const std::unordered_map<std::string, double>& marketPrices) {
//    // Process stop-loss and take-profit orders
//    portfolio_.processStopOrders(marketPrices);
//
//    // Process any pending limit orders
//    portfolio_.processPendingOrders(marketPrices);
//
//    // Update portfolio peak value and drawdown
//    portfolio_.updatePeakValueAndDrawdown(marketPrices);
//}
//
//// Method to get cash balance
//double TradingEngine::getCashBalance() const {
//    return portfolio_.getCashBalance();
//}
//
//// Method to get positions
//const std::unordered_map<std::string, Position>& TradingEngine::getPositions() const {
//    return portfolio_.getPositions();
//}
//
//// Method to get unrealized P&L
//double TradingEngine::getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const {
//    return portfolio_.getUnrealizedPnL(marketPrices);
//}
//
//// Method to get total portfolio value (cash balance + market value of positions)
//double TradingEngine::getTotalPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const {
//    return portfolio_.getCashBalance() + portfolio_.getPortfolioValue(marketPrices);
//}
//
////code added on 2024.10.22
//

// File: src/TradingEngine.cpp

#include "TradingEngine.h"
#include <iostream>

// Constructor
TradingEngine::TradingEngine(double initialCapital)
        : portfolio_(initialCapital) {}

// Set Portfolio (used when loading from persistence)
void TradingEngine::setPortfolio(const Portfolio& portfolio) {
    portfolio_ = portfolio;
}

// Place an order (existing function)
void TradingEngine::placeOrder(const std::string& symbol, OrderType type, OrderStyle style, double price,
                               double stopLossPrice, double takeProfitPrice,
                               const std::unordered_map<std::string, double>& marketPrices) {
    Order order(type, style, symbol, 10, price, stopLossPrice, takeProfitPrice); // Assuming fixed quantity
    portfolio_.executeTrade(order, marketPrices);
}

// User Place Order (new function)
void TradingEngine::userPlaceOrder(const std::string& symbol, OrderType type, OrderStyle style, double price,
                                   double stopLossPrice, double takeProfitPrice,
                                   const std::unordered_map<std::string, double>& marketPrices) {
    // Here you can integrate trading strategies before placing the order
    // For now, it simply places the order
    placeOrder(symbol, type, style, price, stopLossPrice, takeProfitPrice, marketPrices);
}

// Update market data and process orders
void TradingEngine::updateMarketData(const std::unordered_map<std::string, double>& marketPrices) {
    portfolio_.processOrders(marketPrices);
}

// Getter for Portfolio (modifiable)
Portfolio& TradingEngine::getPortfolio() {
    return portfolio_;
}

// Getter for Portfolio (read-only)
const Portfolio& TradingEngine::getPortfolio() const {
    return portfolio_;
}

// Get all orders
std::vector<Order> TradingEngine::getAllOrders() const {
    return portfolio_.getAllOrders();
}

// New Getter: getCashBalance
double TradingEngine::getCashBalance() const {
    return portfolio_.getCashBalance();
}

// New Getter: getPositions
const std::unordered_map<std::string, Position>& TradingEngine::getPositions() const {
    return portfolio_.getPositions();
}

// New Getter: getTotalPortfolioValue
double TradingEngine::getTotalPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const {
    return portfolio_.getPortfolioValue(marketPrices);
}