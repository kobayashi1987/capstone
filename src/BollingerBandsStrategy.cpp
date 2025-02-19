//
// Created by jack on 17/2/25.
//

#include "BollingerBandsStrategy.h"
#include <numeric>
#include <cmath>
#include <stdexcept>

// Constructor: Validate inputs
BollingerBandsStrategy::BollingerBandsStrategy(int period, double numStdDev)
        : period_(period), numStdDev_(numStdDev) {
    if (period_ <= 0 || numStdDev_ <= 0) {
        throw std::invalid_argument("Period and number of standard deviations must be positive.");
    }
}

// Helper function to calculate the Simple Moving Average (SMA)
double BollingerBandsStrategy::calculateSMA(const std::vector<double>& prices, int start, int period) {
    if (start + period > prices.size()) {
        throw std::invalid_argument("Not enough data to calculate SMA.");
    }
    double sum = std::accumulate(prices.begin() + start, prices.begin() + start + period, 0.0);
    return sum / period;
}

// Helper function to calculate standard deviation over a period
double BollingerBandsStrategy::calculateStdDev(const std::vector<double>& prices, int start, int period, double sma) {
    double sumSquares = 0.0;
    for (int i = start; i < start + period; ++i) {
        double diff = prices[i] - sma;
        sumSquares += diff * diff;
    }
    return std::sqrt(sumSquares / period);
}

// Generate trading signals using Bollinger Bands
// For each window of "period" values, compute the SMA and standard deviation.
// If the price at the end of the window is below the lower band, signal a buy (1).
// If above the upper band, signal a sell (-1); otherwise, hold (0).
std::vector<int> BollingerBandsStrategy::generateSignals(const std::vector<double>& prices) {
    int n = prices.size();
    if (n < period_) {
        throw std::invalid_argument("Not enough data to generate signals.");
    }
    // Initialize signals vector with zeros
    std::vector<int> signals(n, 0);
    // Compute signals for each window where we have enough data
    for (int i = 0; i <= n - period_; ++i) {
        double sma = calculateSMA(prices, i, period_);
        double stdDev = calculateStdDev(prices, i, period_, sma);
        double upperBand = sma + numStdDev_ * stdDev;
        double lowerBand = sma - numStdDev_ * stdDev;
        // Use the last price in the current window as the current price
        double currentPrice = prices[i + period_ - 1];
        if (currentPrice < lowerBand) {
            signals[i + period_ - 1] = 1;  // Buy signal
        } else if (currentPrice > upperBand) {
            signals[i + period_ - 1] = -1; // Sell signal
        } else {
            signals[i + period_ - 1] = 0;  // Hold signal
        }
    }
    return signals;
}

// Generate orders based on the signals and a given symbol.
// This method creates a market order for each non-zero signal using a default quantity (e.g., 100 shares).
std::vector<Order> BollingerBandsStrategy::generateOrders(const std::vector<double>& prices, const std::string& symbol) {
    std::vector<Order> orders;
    std::vector<int> signals = generateSignals(prices);
    int defaultQuantity = 100;  // Default quantity for each trade
    // Iterate over the signals; when a buy (1) or sell (-1) signal occurs, create an order.
    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i] == 1 || signals[i] == -1) {
            // Create a market order with placeholder stop-loss and take-profit values (set to 0.0)
            Order order(
                    signals[i] == 1 ? OrderType::Buy : OrderType::Sell,
                    OrderStyle::Market,
                    symbol,
                    defaultQuantity,
                    prices[i],
                    0.0,  // stop-loss placeholder
                    0.0,  // take-profit placeholder
                    std::chrono::system_clock::now()  // use current time as timestamp
            );
            orders.push_back(order);
        }
    }
    return orders;
}