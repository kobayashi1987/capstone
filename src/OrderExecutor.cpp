//
// Created by jack on 9/10/24.
//

#include "OrderExecutor.h"

std::vector<Trade> OrderExecutor::executeOrders(const std::vector<Order>& orders) {
    std::vector<Trade> trades;
    for (const auto& order : orders) {
        // In a real application, there would be logic to match orders.
        // Here, simulating immediate execution.
        trades.emplace_back(order);
    }
    return trades;
}