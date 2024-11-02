////
//// Created by jack on 9/10/24.
////
//
//
//// File: src/Portfolio.cpp
//
//#include "Portfolio.h"
//#include <cmath>        // For std::abs
//#include <algorithm>    // For std::find_if
//#include <iostream>     // For debugging purposes
//#include <stdexcept>    // For exception handling
//
//// Constructor
//Portfolio::Portfolio(double initialCapital)
//        : cashBalance_(initialCapital),
//          peakValue_(initialCapital),
//          drawdown_(0.0) {}
//
//// Setter for Cash Balance
//void Portfolio::setCashBalance(double cash) {
//    cashBalance_ = cash;
//}
//
//// Getter for Cash Balance
//double Portfolio::getCashBalance() const {
//    return cashBalance_;
//}
//
//// Setter for Peak Value
//void Portfolio::setPeakValue(double peakValue) {
//    peakValue_ = peakValue;
//}
//
//// Getter for Peak Value
//double Portfolio::getPeakValue() const {
//    return peakValue_;
//}
//
//// Setter for Drawdown
//void Portfolio::setDrawdown(double drawdown) {
//    drawdown_ = drawdown;
//}
//
//// Getter for Drawdown
//double Portfolio::getDrawdown() const {
//    return drawdown_;
//}
//
//// Getter for Trade History
//const std::vector<Trade>& Portfolio::getTradeHistory() const {
//    return tradeHistory_;
//}
//
//// Getter for Pending Orders
//const std::vector<Order>& Portfolio::getPendingOrders() const {
//    return pendingOrders_;
//}
//
//// Getter for All Orders (Executed and Pending)
//std::vector<Order> Portfolio::getAllOrders() const {
//    std::vector<Order> allOrders = pendingOrders_;
//    for (const auto& trade : tradeHistory_) {
//        allOrders.emplace_back(trade.getOrder());
//    }
//    return allOrders;
//}
//
//// Getter for Positions
//const std::unordered_map<std::string, Position>& Portfolio::getPositions() const {
//    return positions_;
//}
//
//// Getter for Portfolio Value
//double Portfolio::getPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const {
//    double totalValue = cashBalance_;
//    for (const auto& [symbol, position] : positions_) {
//        auto it = marketPrices.find(symbol);
//        if (it != marketPrices.end()) {
//            totalValue += it->second * position.quantity;
//        } else {
//            std::cerr << "Market price for symbol " << symbol << " not found. Using average price as fallback.\n";
//            totalValue += position.averagePrice * position.quantity;
//        }
//    }
//    return totalValue;
//}
//
//// Getter for Unrealized P&L
//double Portfolio::getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const {
//    double unrealizedPnL = 0.0;
//    for (const auto& [symbol, position] : positions_) {
//        auto it = marketPrices.find(symbol);
//        if (it != marketPrices.end()) {
//            unrealizedPnL += (it->second - position.averagePrice) * position.quantity;
//        }
//    }
//    return unrealizedPnL;
//}
//
//// Getter for Position Value
//double Portfolio::getPositionValue(const std::string& symbol, const std::unordered_map<std::string, double>& marketPrices) const {
//    auto it = positions_.find(symbol);
//    if (it != positions_.end()) {
//        auto priceIt = marketPrices.find(symbol);
//        if (priceIt != marketPrices.end()) {
//            return it->second.quantity * priceIt->second;
//        } else {
//            std::cerr << "Market price for symbol " << symbol << " not found. Using average price as fallback.\n";
//            return it->second.quantity * it->second.averagePrice;
//        }
//    } else {
//        std::cerr << "Position for symbol " << symbol << " not found.\n";
//        return 0.0;
//    }
//}
//
//// Add or Update a Position
//void Portfolio::addPosition(const std::string& symbol, int quantity, double price) {
//    auto it = positions_.find(symbol);
//    if (it != positions_.end()) {
//        // Update existing position
//        double totalCost = it->second.averagePrice * it->second.quantity + price * quantity;
//        it->second.quantity += quantity;
//        if (it->second.quantity != 0) {
//            it->second.averagePrice = totalCost / it->second.quantity;
//        } else {
//            positions_.erase(it); // Remove position if quantity is zero
//        }
//    } else {
//        // Add new position
//        positions_[symbol] = Position{ quantity, price };
//    }
//}
//
//// Add a Trade to Trade History
//void Portfolio::addTrade(const Trade& trade) {
//    tradeHistory_.push_back(trade);
//}
//
//// Add a Pending Order
//void Portfolio::addPendingOrder(const Order& order) {
//    pendingOrders_.push_back(order);
//}
//
//// Update a Position
//void Portfolio::updatePosition(const std::string& symbol, int quantity, double averagePrice) {
//    auto it = positions_.find(symbol);
//    if (it != positions_.end()) {
//        it->second.quantity = quantity;
//        it->second.averagePrice = averagePrice;
//    } else {
//        Position pos;
//        pos.quantity = quantity;
//        pos.averagePrice = averagePrice;
//        positions_[symbol] = pos;
//    }
//}
//
//// Method to Place an Order
//void Portfolio::placeOrder(const Order& order, const std::unordered_map<std::string, double>& marketPrices) {
//    // For simplicity, we'll assume Market Orders are executed immediately
//    // and Limit Orders are added to pendingOrders_
//    if (order.getStyle() == OrderStyle::Market) {
//        // Execute the order immediately
//        executeTrade(order, marketPrices);
//    }
//    else if (order.getStyle() == OrderStyle::Limit) {
//        // Add to pending orders
//        pendingOrders_.push_back(order);
//        std::cout << "Limit order added to pending orders.\n";
//    }
//}
//
//// Method to Execute a Trade
//void Portfolio::executeTrade(const Order& order, const std::unordered_map<std::string, double>& marketPrices) {
//    // Create the Trade object and add to trade history
//    Trade trade(order);
//    tradeHistory_.push_back(trade);
//
//    // Update positions and cash balance
//    if (order.getType() == OrderType::Buy) {
//        cashBalance_ -= (order.getPrice() * order.getQuantity());
//
//        // Update position
//        auto it = positions_.find(order.getSymbol());
//        if (it != positions_.end()) {
//            // Update average price
//            double totalCost = it->second.averagePrice * it->second.quantity;
//            totalCost += order.getPrice() * order.getQuantity();
//            it->second.quantity += order.getQuantity();
//            it->second.averagePrice = totalCost / it->second.quantity;
//        }
//        else {
//            // New position
//            Position pos;
//            pos.quantity = order.getQuantity();
//            pos.averagePrice = order.getPrice();
//            positions_[order.getSymbol()] = pos;
//        }
//    }
//    else if (order.getType() == OrderType::Sell) {
//        cashBalance_ += (order.getPrice() * order.getQuantity());
//
//        // Update position
//        auto it = positions_.find(order.getSymbol());
//        if (it != positions_.end()) {
//            it->second.quantity -= order.getQuantity();
//            if (it->second.quantity <= 0) {
//                // Remove the position if quantity is zero or negative
//                positions_.erase(it);
//            }
//        }
//        else {
//            std::cerr << "Error: Attempting to sell a non-existent position for symbol: " << order.getSymbol() << "\n";
//        }
//    }
//
//    // Calculate total portfolio value
//    double totalPortfolioValue = getPortfolioValue(marketPrices);
//
//    // Update peak value and drawdown
//    updatePeakValueAndDrawdown(totalPortfolioValue);
//
//    std::cout << "Trade executed: " << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
//              << " " << order.getQuantity() << " shares of " << order.getSymbol()
//              << " at $" << order.getPrice() << "\n";
//}
//
//// Method to Process Pending Orders
//void Portfolio::processOrders(const std::unordered_map<std::string, double>& marketPrices) {
//    if (pendingOrders_.empty()) {
//        std::cout << "No pending orders to process.\n";
//        return;
//    }
//
//    std::vector<Order> executedOrders;
//
//    for (const auto& order : pendingOrders_) {
//        bool execute = false;
//
//        // Check execution conditions based on order type
//        if (order.getType() == OrderType::Buy) {
//            // Buy Limit Order: Execute if market price <= limit price
//            auto it = marketPrices.find(order.getSymbol());
//            if (it != marketPrices.end() && it->second <= order.getPrice()) {
//                execute = true;
//            }
//        }
//        else if (order.getType() == OrderType::Sell) {
//            // Sell Limit Order: Execute if market price >= limit price
//            auto it = marketPrices.find(order.getSymbol());
//            if (it != marketPrices.end() && it->second >= order.getPrice()) {
//                execute = true;
//            }
//        }
//
//        if (execute) {
//            try {
//                executeTrade(order, marketPrices);
//                std::cout << "Executed pending order: "
//                          << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
//                          << " " << order.getQuantity() << " shares of " << order.getSymbol()
//                          << " at $" << order.getPrice() << "\n";
//                executedOrders.emplace_back(order);
//            }
//            catch (const std::exception& e) {
//                std::cout << "Failed to execute order for " << order.getSymbol() << ": " << e.what() << "\n";
//            }
//        }
//    }
//
//    // Remove executed orders from pendingOrders_
//    for (const auto& execOrder : executedOrders) {
//        auto it = std::find_if(pendingOrders_.begin(), pendingOrders_.end(),
//                               [&execOrder](const Order& o) {
//                                   return o.getTimestamp() == execOrder.getTimestamp() &&
//                                          o.getSymbol() == execOrder.getSymbol() &&
//                                          o.getType() == execOrder.getType() &&
//                                          o.getPrice() == execOrder.getPrice();
//                               });
//        if (it != pendingOrders_.end()) {
//            pendingOrders_.erase(it);
//        }
//    }
//}
//
//// Method to Process Stop-Loss and Take-Profit Orders
//void Portfolio::processStopOrders(const std::unordered_map<std::string, double>& marketPrices) {
//    // Iterate through positions to check for stop-loss or take-profit triggers
//    std::vector<Order> triggeredStopOrders;
//
//    for (const auto& [symbol, position] : positions_) {
//        auto it = marketPrices.find(symbol);
//        if (it == marketPrices.end()) {
//            std::cerr << "Market price for symbol " << symbol << " not found.\n";
//            continue;
//        }
//        double currentPrice = it->second;
//
//        // Example logic: If currentPrice <= stopLossPrice or >= takeProfitPrice, execute sell or buy respectively
//        // This requires storing stop-loss and take-profit prices, which are not currently part of the Position struct
//        // For demonstration, we'll assume stop-loss and take-profit are part of pendingOrders_
//
//        // Find corresponding stop-loss or take-profit orders
//        for (const auto& order : pendingOrders_) {
//            if (order.getSymbol() == symbol) {
//                if (order.getType() == OrderType::Sell && currentPrice <= order.getStopLoss()) {
//                    triggeredStopOrders.push_back(order);
//                }
//                else if (order.getType() == OrderType::Buy && currentPrice >= order.getTakeProfit()) {
//                    triggeredStopOrders.push_back(order);
//                }
//            }
//        }
//    }
//
//    // Execute triggered stop orders
//    for (const auto& order : triggeredStopOrders) {
//        executeTrade(order, marketPrices);
//    }
//
//    // Remove executed stop orders from pending orders
//    pendingOrders_.erase(
//            std::remove_if(pendingOrders_.begin(), pendingOrders_.end(),
//                           [&triggeredStopOrders](const Order& o) {
//                               return std::find(triggeredStopOrders.begin(), triggeredStopOrders.end(), o) != triggeredStopOrders.end();
//                           }),
//            pendingOrders_.end());
//}
//
//// Helper Method to Update Peak Value and Drawdown
//void Portfolio::updatePeakValueAndDrawdown(double totalPortfolioValue) {
//    if (totalPortfolioValue > peakValue_) {
//        peakValue_ = totalPortfolioValue;
//        drawdown_ = 0.0;
//    }
//    else {
//        drawdown_ = (peakValue_ - totalPortfolioValue) / peakValue_;
//    }
//}

// File: src/Portfolio.cpp

#include "Portfolio.h"
#include "ProfitLoss.h"
#include <cmath>        // For std::abs
#include <algorithm>    // For std::find_if
#include <iostream>     // For debugging purposes
#include <iomanip>      // For std::put_time
#include <stdexcept>    // For exception handling
#include <sstream>      // Newly added for std::ostringstream

// Constructor
Portfolio::Portfolio(double initialCapital)
        : cashBalance_(initialCapital),
          peakValue_(initialCapital),
          drawdown_(0.0) {}

// Setter for Cash Balance
void Portfolio::setCashBalance(double cash) {
    cashBalance_ = cash;
}

// Getter for Cash Balance
double Portfolio::getCashBalance() const {
    return cashBalance_;
}

// Setter for Peak Value
void Portfolio::setPeakValue(double peakValue) {
    peakValue_ = peakValue;
}

// Getter for Peak Value
double Portfolio::getPeakValue() const {
    return peakValue_;
}

// Setter for Drawdown
void Portfolio::setDrawdown(double drawdown) {
    drawdown_ = drawdown;
}

// Getter for Drawdown
double Portfolio::getDrawdown() const {
    return drawdown_;
}

// Getter for Trade History
const std::vector<Trade>& Portfolio::getTradeHistory() const {
    return tradeHistory_;
}

// Getter for Pending Orders
const std::vector<Order>& Portfolio::getPendingOrders() const {
    return pendingOrders_;
}

// Getter for Positions
const std::unordered_map<std::string, Position>& Portfolio::getPositions() const {
    return positions_;
}

// Getter for Portfolio Value
double Portfolio::getPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const {
    double totalValue = cashBalance_;
    for (const auto& [symbol, position] : positions_) {
        auto it = marketPrices.find(symbol);
        if (it != marketPrices.end()) {
            totalValue += it->second * position.quantity;
        } else {
            std::cerr << "Market price for symbol " << symbol << " not found. Using average price as fallback.\n";
            totalValue += position.averagePrice * position.quantity;
        }
    }
    return totalValue;
}

// Getter for Unrealized P&L
double Portfolio::getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const {
    double unrealizedPnL = 0.0;
    for (const auto& [symbol, position] : positions_) {
        auto it = marketPrices.find(symbol);
        if (it != marketPrices.end()) {
            unrealizedPnL += (it->second - position.averagePrice) * position.quantity;
        }
    }
    return unrealizedPnL;
}

// Getter for Profit & Loss List
const std::vector<ProfitLoss>& Portfolio::getProfitLossList() const {
    return plList_;
}

// Setter for Profit & Loss List
void Portfolio::setProfitLossList(const std::vector<ProfitLoss>& plList) {
    plList_ = plList;
}

// Method to add a Profit & Loss entry
void Portfolio::addProfitLoss(const ProfitLoss& pl) {
    plList_.push_back(pl);
}

// Add or Update a Position
void Portfolio::addPosition(const std::string& symbol, int quantity, double price) {
    auto it = positions_.find(symbol);
    if (it != positions_.end()) {
        // Update existing position
        double totalCost = it->second.averagePrice * it->second.quantity;
        totalCost += price * quantity;
        it->second.quantity += quantity;
        if (it->second.quantity != 0) {
            it->second.averagePrice = totalCost / it->second.quantity;
        } else {
            positions_.erase(it); // Remove position if quantity is zero
        }
    } else {
        // Add new position
        positions_[symbol] = Position{ quantity, price };
    }
}

// Add a Trade to Trade History
void Portfolio::addTrade(const Trade& trade) {
    tradeHistory_.push_back(trade);
}

// Add a Pending Order
void Portfolio::addPendingOrder(const Order& order) {
    pendingOrders_.push_back(order);
}

// Update a Position
void Portfolio::updatePosition(const std::string& symbol, int quantity, double averagePrice) {
    auto it = positions_.find(symbol);
    if (it != positions_.end()) {
        it->second.quantity = quantity;
        it->second.averagePrice = averagePrice;
    } else {
        Position pos;
        pos.quantity = quantity;
        pos.averagePrice = averagePrice;
        positions_[symbol] = pos;
    }
}

// Method to place an order
void Portfolio::placeOrder(const Order& order, const std::unordered_map<std::string, double>& marketPrices) {
    // For simplicity, we'll assume Market Orders are executed immediately
    // and Limit Orders are added to pendingOrders_
    if (order.getStyle() == OrderStyle::Market) {
        // Execute the order immediately
        executeTrade(order, marketPrices);
    }
    else if (order.getStyle() == OrderStyle::Limit) {
        // Add to pending orders
        pendingOrders_.push_back(order);
        std::cout << "Limit order added to pending orders.\n";
    }
}


// Method to execute a trade
// Added detailed console outputs to inform the user about the execution status,
// including successful trades and specific error reasons.
void Portfolio::executeTrade(const Order& order, const std::unordered_map<std::string, double>& marketPrices) {
    // Handle Buy orders
    if (order.getType() == OrderType::Buy) {
        cashBalance_ -= (order.getPrice() * order.getQuantity());

        // Update or add the position
        auto it = positions_.find(order.getSymbol());
        if (it != positions_.end()) {
            // Update existing position's average price
            double totalCost = it->second.averagePrice * it->second.quantity;
            totalCost += order.getPrice() * order.getQuantity();
            it->second.quantity += order.getQuantity();
            it->second.averagePrice = totalCost / it->second.quantity;
        }
        else {
            // Add new position
            positions_[order.getSymbol()] = Position{ order.getQuantity(), order.getPrice() };
        }

        std::cout << "Buy order executed: " << order.getQuantity() << " shares of " << order.getSymbol()
                  << " at $" << order.getPrice() << "\n";
    }
        // Handle Sell orders
    else if (order.getType() == OrderType::Sell) {
        auto it = positions_.find(order.getSymbol());
        if (it != positions_.end()) {
            if (it->second.quantity >= order.getQuantity()) {
                // Proceed with selling logic
                it->second.quantity -= order.getQuantity();
                if (it->second.quantity == 0) {
                    positions_.erase(it);
                }

                // Calculate P&L
                double averageBuyPrice = it->second.averagePrice;
                double profit_loss = (order.getPrice() - averageBuyPrice) * order.getQuantity();

                // Convert timestamp to string
                std::time_t time = std::chrono::system_clock::to_time_t(order.getTimestamp());
                std::ostringstream oss;
                oss << std::put_time(std::localtime(&time), "%Y-%m-%dT%H:%M:%S");
                std::string timestampStr = oss.str();

                // Create ProfitLoss entry
                ProfitLoss pl;
                pl.symbol = order.getSymbol();
                pl.quantity = order.getQuantity();
                pl.sell_price = order.getPrice();
                pl.average_buy_price = averageBuyPrice;
                pl.profit_loss = profit_loss;
                pl.timestamp = timestampStr;

                // Add to P&L list
                addProfitLoss(pl);

                // Update cash balance
                // After selling, the cash balance is increased by the total sale amount.
                cashBalance_ += (order.getPrice() * order.getQuantity());

                std::cout << "Sell order executed: " << order.getQuantity() << " shares of " << order.getSymbol()
                          << " at $" << order.getPrice() << "\n"
                          << "Profit/Loss: $" << profit_loss << "\n";
            }
            else {
                // Insufficient shares to sell
                // Before executing a sell, verify that the portfolio holds at least the number of shares intended to sell.
                std::cerr << "Error: Attempting to sell " << order.getQuantity() << " shares of "
                          << order.getSymbol() << ", but only " << it->second.quantity << " shares are available.\n";
            }
        }
        else {
            // No existing position to sell
            std::cerr << "Error: Attempting to sell a non-existent position for symbol: " << order.getSymbol() << "\n";
        }
    }

    // Update peak value and drawdown
    // After executing the trade, recalculate the portfolio’s peak value and drawdown.
    double totalPortfolioValue = getPortfolioValue(marketPrices);
    updatePeakValueAndDrawdown(totalPortfolioValue);
}

// Method to process pending orders
void Portfolio::processOrders(const std::unordered_map<std::string, double>& marketPrices) {
    if (pendingOrders_.empty()) {
        std::cout << "No pending orders to process.\n";
        return;
    }

    std::vector<Order> executedOrders;

    for (const auto& order : pendingOrders_) {
        bool execute = false;

        // Check execution conditions based on order type
        if (order.getType() == OrderType::Buy) {
            // Buy Limit Order: Execute if market price <= limit price
            auto it = marketPrices.find(order.getSymbol());
            if (it != marketPrices.end() && it->second <= order.getPrice()) {
                execute = true;
            }
        }
        else if (order.getType() == OrderType::Sell) {
            // Sell Limit Order: Execute if market price >= limit price
            auto it = marketPrices.find(order.getSymbol());
            if (it != marketPrices.end() && it->second >= order.getPrice()) {
                execute = true;
            }
        }

        if (execute) {
            try {
                executeTrade(order, marketPrices);
                std::cout << "Executed pending order: "
                          << (order.getType() == OrderType::Buy ? "Buy" : "Sell")
                          << " " << order.getQuantity() << " shares of " << order.getSymbol()
                          << " at $" << order.getPrice() << "\n";
                executedOrders.emplace_back(order);
            }
            catch (const std::exception& e) {
                std::cout << "Failed to execute order for " << order.getSymbol() << ": " << e.what() << "\n";
            }
        }
    }

    // Remove executed orders from pendingOrders_
    for (const auto& execOrder : executedOrders) {
        auto it = std::find_if(pendingOrders_.begin(), pendingOrders_.end(),
                               [&execOrder](const Order& o) {
                                   return o.getTimestamp() == execOrder.getTimestamp() &&
                                          o.getSymbol() == execOrder.getSymbol() &&
                                          o.getType() == execOrder.getType() &&
                                          o.getPrice() == execOrder.getPrice() &&
                                          o.getQuantity() == execOrder.getQuantity();
                               });
        if (it != pendingOrders_.end()) {
            pendingOrders_.erase(it);
        }
    }
}

// Method to process stop-loss and take-profit orders
void Portfolio::processStopOrders(const std::unordered_map<std::string, double>& marketPrices) {
    // Iterate through positions to check for stop-loss or take-profit triggers
    std::vector<Order> triggeredStopOrders;

    for (const auto& [symbol, position] : positions_) {
        auto it = marketPrices.find(symbol);
        if (it == marketPrices.end()) {
            std::cerr << "Market price for symbol " << symbol << " not found.\n";
            continue;
        }
        double currentPrice = it->second;

        // Find corresponding stop-loss or take-profit orders
        for (const auto& order : pendingOrders_) {
            if (order.getSymbol() == symbol) {
                if (order.getType() == OrderType::Sell && currentPrice <= order.getStopLoss()) {
                    triggeredStopOrders.push_back(order);
                }
                else if (order.getType() == OrderType::Buy && currentPrice >= order.getTakeProfit()) {
                    triggeredStopOrders.push_back(order);
                }
            }
        }
    }

    // Execute triggered stop orders
    for (const auto& order : triggeredStopOrders) {
        executeTrade(order, marketPrices);
    }

    // Remove executed stop orders from pending orders
    pendingOrders_.erase(
            std::remove_if(pendingOrders_.begin(), pendingOrders_.end(),
                           [&triggeredStopOrders](const Order& o) {
                               return std::find(triggeredStopOrders.begin(), triggeredStopOrders.end(), o) != triggeredStopOrders.end();
                           }),
            pendingOrders_.end());
}

// Helper Method to Update Peak Value and Drawdown
void Portfolio::updatePeakValueAndDrawdown(double totalPortfolioValue) {
    if (totalPortfolioValue > peakValue_) {
        peakValue_ = totalPortfolioValue;
        drawdown_ = 0.0;
    }
    else {
        drawdown_ = (peakValue_ - totalPortfolioValue) / peakValue_;
    }
}