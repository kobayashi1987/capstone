//
// Created by jack on 23/10/24.
//
// File: src/DataPersistence.h

#ifndef DATAPERSISTENCE_H
#define DATAPERSISTENCE_H

#include "Portfolio.h"
#include "Order.h"
#include "json.hpp"
#include <string>
#include <vector>

class DataPersistence {
public:
    // Constructor
    DataPersistence(const std::string& portfolioFilePath, const std::string& ordersFilePath);

    // Save and load methods for Portfolio
    bool savePortfolio(const Portfolio& portfolio) const;
    bool loadPortfolio(Portfolio& portfolio) const;

    // Save and load methods for Orders
    bool saveOrders(const std::vector<Order>& orders) const;
    bool loadOrders(std::vector<Order>& orders) const;

private:
    std::string portfolioFilePath_;
    std::string ordersFilePath_;
};

#endif // DATAPERSISTENCE_H