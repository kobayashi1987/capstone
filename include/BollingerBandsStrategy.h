#ifndef BOLLINGERBANDSSTRATEGY_H
#define BOLLINGERBANDSSTRATEGY_H

#include <vector>
#include <string>
#include "Order.h"

// BollingerBandsStrategy:
// - period: number of periods to calculate the moving average and standard deviation
// - numStdDev: number of standard deviations to set the bands
class BollingerBandsStrategy {
public:
    BollingerBandsStrategy(int period, double numStdDev);

    // Generate trading signals from a vector of prices.
    // Returns a vector of signals (1 = buy, -1 = sell, 0 = hold).
    std::vector<int> generateSignals(const std::vector<double>& prices);

    // Generate orders based on the generated signals and a given symbol.
    // For simplicity, this function uses a default quantity (e.g. 100 shares)
    // and creates market orders with placeholder stop-loss and take-profit values.
    std::vector<Order> generateOrders(const std::vector<double>& prices, const std::string& symbol);

private:
    int period_;
    double numStdDev_;

    // Helper functions to calculate the simple moving average (SMA) and standard deviation.
    double calculateSMA(const std::vector<double>& prices, int start, int period);
    double calculateStdDev(const std::vector<double>& prices, int start, int period, double sma);
};

#endif // BOLLINGERBANDSSTRATEGY_H