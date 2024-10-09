//
// Created by jack on 7/10/24.
//

#ifndef RSISTRATEGY_H
#define RSISTRATEGY_H

#include "TradingStrategy.h"
#include <vector>

class RSIStrategy : public TradingStrategy {
public:
    RSIStrategy(int period, double overbought = 70.0, double oversold = 30.0);
    void generateSignals(const std::vector<double>& prices) override;

private:
    int period_;
    double overbought_;
    double oversold_;

    std::vector<double> calculateRSI(const std::vector<double>& prices);
};

#endif // RSISTRATEGY_H