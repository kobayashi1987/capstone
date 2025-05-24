//
// Created by jack on 10/10/24.
//

#ifndef MOMENTUMSTRATEGY_H
#define MOMENTUMSTRATEGY_H

#include "TradingStrategy.h"
#include <vector>

class MomentumStrategy : public TradingStrategy {
public:
    MomentumStrategy(int lookbackPeriod, double buyThreshold = 0.05, double sellThreshold = -0.05);

    void generateSignals(const std::vector<double>& prices) override;
    std::vector<Order> generateOrders(const std::vector<double>& prices, const std::string& symbol) override;

    // Accessor methods to retrieve momentum values and signals
    const std::vector<double>& getMomentumValues() const;
    const std::vector<int>& getSignals() const;

private:
    int lookbackPeriod_;
    double buyThreshold_;
    double sellThreshold_;
    std::vector<double> momentumValues_;
    std::vector<int> signals_;
};

#endif // MOMENTUMSTRATEGY_H