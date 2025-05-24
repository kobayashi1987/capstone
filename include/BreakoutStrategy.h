//
// Created by jack on 10/10/24.
//

#ifndef BREAKOUTSTRATEGY_H
#define BREAKOUTSTRATEGY_H

#include "TradingStrategy.h"
#include <vector>

class BreakoutStrategy : public TradingStrategy {
public:
    BreakoutStrategy(int lookbackPeriod);

    void generateSignals(const std::vector<double>& prices) override;
    std::vector<Order> generateOrders(const std::vector<double>& prices, const std::string& symbol) override;

    // Accessor methods to retrieve signals and levels
    const std::vector<int>& getSignals() const;
    const std::vector<double>& getResistanceLevels() const;
    const std::vector<double>& getSupportLevels() const;

private:
    int lookbackPeriod_;
    std::vector<int> signals_;
    std::vector<double> resistanceLevels_;
    std::vector<double> supportLevels_;
};

#endif // BREAKOUTSTRATEGY_H