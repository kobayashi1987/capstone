////
//// Created by jack on 9/10/24.
////


#include "Portfolio.h"
#include "Order.h"
#include "Trade.h"
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm> // For std::max


// Constructor: Initializes the Portfolio with the initial cash balance
Portfolio::Portfolio(double initialCash)
        : cashBalance_(initialCash), peakValue_(initialCash), drawdown_(0.0) {
    // Initialize positions, pendingOrders, and tradeHistory as empty
}

// Method to place an order
void Portfolio::placeOrder(const Order& order, const std::unordered_map<std::string, double>& marketPrices) {
    // For simplicity, we'll assume market orders are executed immediately
    if (order.getStyle() == OrderStyle::Market) {
        // Create a trade from the order
        Trade trade(order);
        // Update the portfolio with the executed trade
        std::vector<Trade> executedTrades = { trade };
        updatePortfolio(executedTrades, marketPrices);
    } else {
        // For limit orders, add to pending orders
        pendingOrders_.push_back(order);
    }
}

// Method to process pending limit orders
void Portfolio::processPendingOrders(const std::unordered_map<std::string, double>& marketPrices) {
    std::vector<Trade> executedTrades;
    auto it = pendingOrders_.begin();
    while (it != pendingOrders_.end()) {
        const Order& order = *it;
        const std::string& symbol = order.getSymbol();
        double marketPrice = marketPrices.at(symbol);
        bool executed = false;

        if (order.getType() == OrderType::Buy) {
            // For buy limit orders, execute if market price <= order price
            if (marketPrice <= order.getPrice()) {
                Trade trade(order);
                executedTrades.push_back(trade);
                executed = true;
            }
        } else {
            // For sell limit orders, execute if market price >= order price
            if (marketPrice >= order.getPrice()) {
                Trade trade(order);
                executedTrades.push_back(trade);
                executed = true;
            }
        }

        if (executed) {
            it = pendingOrders_.erase(it);
        } else {
            ++it;
        }
    }

    if (!executedTrades.empty()) {
        updatePortfolio(executedTrades, marketPrices);
    }
}

// Method to process stop-loss and take-profit orders
void Portfolio::processStopOrders(const std::unordered_map<std::string, double>& marketPrices) {
    std::vector<Trade> executedTrades;

    for (auto it = positions_.begin(); it != positions_.end();) {
        const std::string& symbol = it->first;
        Position& position = it->second;
        double marketPrice = marketPrices.at(symbol);

        bool positionClosed = false;

        // Check for stop-loss
        if (position.stopLossPrice > 0.0) {
            if ((position.quantity > 0 && marketPrice <= position.stopLossPrice) ||
                (position.quantity < 0 && marketPrice >= position.stopLossPrice)) {
                // Close position
                int closeQuantity = -position.quantity;
                OrderType orderType = position.quantity > 0 ? OrderType::Sell : OrderType::Buy;
                Order stopOrder(orderType, OrderStyle::Market, symbol, std::abs(position.quantity), marketPrice);
                Trade trade(stopOrder);
                executedTrades.push_back(trade);

                std::cout << "Stop-loss triggered for " << symbol << " at $" << marketPrice << "\n";

                // Remove position
                it = positions_.erase(it);
                positionClosed = true;
            }
        }

        // Check for take-profit
        if (!positionClosed && position.takeProfitPrice > 0.0) {
            if ((position.quantity > 0 && marketPrice >= position.takeProfitPrice) ||
                (position.quantity < 0 && marketPrice <= position.takeProfitPrice)) {
                // Close position
                int closeQuantity = -position.quantity;
                OrderType orderType = position.quantity > 0 ? OrderType::Sell : OrderType::Buy;
                Order tpOrder(orderType, OrderStyle::Market, symbol, std::abs(position.quantity), marketPrice);
                Trade trade(tpOrder);
                executedTrades.push_back(trade);

                std::cout << "Take-profit triggered for " << symbol << " at $" << marketPrice << "\n";

                // Remove position
                it = positions_.erase(it);
                positionClosed = true;
            }
        }

        // Move to next position if not closed
        if (!positionClosed) {
            ++it;
        }
    }

    if (!executedTrades.empty()) {
        updatePortfolio(executedTrades, marketPrices);
    }
}

// Method to update the portfolio with executed trades
void Portfolio::updatePortfolio(const std::vector<Trade>& executedTrades, const std::unordered_map<std::string, double>& marketPrices) {
    for (const auto& trade : executedTrades) {
        const std::string& symbol = trade.getSymbol();
        int quantity = trade.getQuantity();
        double price = trade.getPrice();

        // Update cash balance
        double tradeAmount = price * quantity;
        cashBalance_ -= tradeAmount;

        // Update positions
        auto it = positions_.find(symbol);
        if (it != positions_.end()) {
            // Update existing position
            Position& position = it->second;
            int newQuantity = position.quantity + quantity;
            if (newQuantity == 0) {
                // Position closed
                positions_.erase(it);
            } else {
                // Update average price
                position.averagePrice = ((position.averagePrice * position.quantity) + (price * quantity)) / newQuantity;
                position.quantity = newQuantity;
                // Update stop-loss and take-profit prices
                position.stopLossPrice = trade.getStopLoss();
                position.takeProfitPrice = trade.getTakeProfit();
            }
        } else {
            // Create new position using the updated constructor
            positions_[symbol] = Position(quantity, price, trade.getStopLoss(), trade.getTakeProfit());
        }

        // Add trade to trade history
        tradeHistory_.push_back(trade);
    }

    // Update peak value and drawdown after the portfolio change
    updatePeakValueAndDrawdown(marketPrices);
}

// Method to update peak portfolio value and drawdown
void Portfolio::updatePeakValueAndDrawdown(const std::unordered_map<std::string, double>& marketPrices) {
    double currentValue = getCashBalance() + getPortfolioValue(marketPrices);
    if (currentValue > peakValue_) {
        peakValue_ = currentValue;
    }
    drawdown_ = (peakValue_ - currentValue) / peakValue_;
}

// Method to get cash balance
double Portfolio::getCashBalance() const {
    return cashBalance_;
}

// Method to get the total market value of all positions
double Portfolio::getPortfolioValue(const std::unordered_map<std::string, double>& marketPrices) const {
    double totalValue = 0.0;
    for (const auto& positionPair : positions_) {
        const std::string& symbol = positionPair.first;
        const Position& position = positionPair.second;
        double marketPrice = marketPrices.at(symbol);
        totalValue += position.quantity * marketPrice;
    }
    return totalValue;
}

// Method to get positions
const std::unordered_map<std::string, Position>& Portfolio::getPositions() const {
    return positions_;
}

// Method to get unrealized P&L
double Portfolio::getUnrealizedPnL(const std::unordered_map<std::string, double>& marketPrices) const {
    double unrealizedPnL = 0.0;
    for (const auto& positionPair : positions_) {
        const std::string& symbol = positionPair.first;
        const Position& position = positionPair.second;
        double marketPrice = marketPrices.at(symbol);
        double positionPnL = (marketPrice - position.averagePrice) * position.quantity;
        unrealizedPnL += positionPnL;
    }
    return unrealizedPnL;
}

// Method to get position value for a specific symbol
double Portfolio::getPositionValue(const std::string& symbol, const std::unordered_map<std::string, double>& marketPrices) const {
    auto it = positions_.find(symbol);
    if (it != positions_.end()) {
        const Position& position = it->second;
        double marketPrice = marketPrices.at(symbol);
        return position.quantity * marketPrice;
    }
    return 0.0;
}

// Method to get the peak portfolio value
double Portfolio::getPeakValue() const {
    return peakValue_;
}

// Method to get the current drawdown
double Portfolio::getDrawdown() const {
    return drawdown_;
}

// Method to set the peak portfolio value (used during initialization)
void Portfolio::setPeakValue(double value) {
    peakValue_ = value;
}

// Additional methods can be added as necessary, depending on further requirements.

