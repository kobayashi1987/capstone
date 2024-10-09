//
// Created by jack on 9/10/24.
//

#include "Trade.h"

Trade::Trade(const Order& order)
        : type_(order.getType()), symbol_(order.getSymbol()), quantity_(order.getQuantity()),
          price_(order.getPrice()), timestamp_(order.getTimestamp()) {}

OrderType Trade::getType() const { return type_; }
const std::string& Trade::getSymbol() const { return symbol_; }
int Trade::getQuantity() const { return quantity_; }
double Trade::getPrice() const { return price_; }
std::chrono::system_clock::time_point Trade::getTimestamp() const { return timestamp_; }