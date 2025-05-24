////
//// Created by jack on 7/10/24.
////

// RSIStrategy.h

#ifndef RSISTRATEGY_H
#define RSISTRATEGY_H

#include "TradingStrategy.h"
#include <vector>

class RSIStrategy : public TradingStrategy {
public:
    RSIStrategy(int period, double overboughtThreshold = 70.0, double oversoldThreshold = 30.0);

    void generateSignals(const std::vector<double>& prices) override;

    std::vector<Order> generateOrders(const std::vector<double>& prices, const std::string& symbol) override;

    // Accessor methods to retrieve RSI values and signals
    const std::vector<double>& getRSIValues() const;
    const std::vector<int>& getSignals() const;


private:
    int period_;
    double overboughtThreshold_;
    double oversoldThreshold_;
    std::vector<double> rsiValues_;
    std::vector<int> signals_;
};

#endif // RSISTRATEGY_H