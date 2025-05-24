//
// Created by jack on 7/10/24.
//

// include/TradingStrategy.h

#ifndef TRADINGSTRATEGY_H
#define TRADINGSTRATEGY_H

#include <vector>

#include "Order.h"

class TradingStrategy {
public:
    virtual ~TradingStrategy() = default;

    // Pure virtual function to generate trading signals
    virtual void generateSignals(const std::vector<double>& prices) = 0;

    // New method to generate orders
    virtual std::vector<Order> generateOrders(const std::vector<double>& prices, const std::string& symbol);


    // Function to retrieve generated signals
    const std::vector<int>& getSignals() const;

protected:
    std::vector<int> signals_;
};

#endif // TRADINGSTRATEGY_H