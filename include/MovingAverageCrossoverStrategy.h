//
// Created by jack on 7/10/24.
//

// include/MovingAverageCrossoverStrategy.h

#ifndef MOVINGAVERAGECROSSOVERSTRATEGY_H
#define MOVINGAVERAGECROSSOVERSTRATEGY_H

#include "TradingStrategy.h"
#include "Order.h"
#include <vector>

class MovingAverageCrossoverStrategy : public TradingStrategy {
public:
    MovingAverageCrossoverStrategy(int shortWindow, int longWindow);
    void generateSignals(const std::vector<double>& prices) override;

    // Override generateOrders
    std::vector<Order> generateOrders(const std::vector<double>& prices, const std::string& symbol) override;


private:
    int shortWindow_;
    int longWindow_;

    // Helper functions
    double calculateMovingAverage(const std::vector<double>& prices, size_t startIndex, int windowSize);
};

#endif // MOVINGAVERAGECROSSOVERSTRATEGY_H