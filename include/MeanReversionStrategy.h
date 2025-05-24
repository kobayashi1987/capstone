//
// Created by jack on 20/10/24.
//

#ifndef MEANREVERSIONSTRATEGY_H
#define MEANREVERSIONSTRATEGY_H

#include "TradingStrategy.h"
#include <vector>

class MeanReversionStrategy : public TradingStrategy {
public:
    MeanReversionStrategy(int lookbackPeriod, double deviationThreshold);

    void generateSignals(const std::vector<double>& prices) override;
    std::vector<Order> generateOrders(const std::vector<double>& prices, const std::string& symbol) override;

    // Accessor methods to retrieve signals and mean values
    const std::vector<int>& getSignals() const;
    const std::vector<double>& getMeanValues() const;

private:
    int lookbackPeriod_;
    double deviationThreshold_;
    std::vector<int> signals_;
    std::vector<double> meanValues_;
};

#endif // MEANREVERSIONSTRATEGY_H