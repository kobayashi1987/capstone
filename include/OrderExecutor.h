//
// Created by jack on 9/10/24.
//

#ifndef ORDEREXECUTOR_H
#define ORDEREXECUTOR_H

#include "Order.h"
#include "Trade.h"
#include <vector>

class OrderExecutor {
public:
    // Simulate executing orders and return trades
    std::vector<Trade> executeOrders(const std::vector<Order>& orders);
};

#endif // ORDEREXECUTOR_H