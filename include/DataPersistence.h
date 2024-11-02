////
//// Created by jack on 23/10/24.
////
//// File: src/DataPersistence.h
//
//#ifndef DATAPERSISTENCE_H
//#define DATAPERSISTENCE_H
//
//#include "Portfolio.h"
//#include "Order.h"
//#include "json.hpp"
//#include <string>
//#include <vector>
//
//class DataPersistence {
//public:
//    // Constructor
//    DataPersistence(const std::string& portfolioFilePath, const std::string& ordersFilePath);
//
//    // Save and load methods for Portfolio
//    bool savePortfolio(const Portfolio& portfolio) const;
//    bool loadPortfolio(Portfolio& portfolio) const;
//
//    // Save and load methods for Orders
//    bool saveOrders(const std::vector<Order>& orders) const;
//    bool loadOrders(std::vector<Order>& orders) const;
//
//private:
//    std::string portfolioFilePath_;
//    std::string ordersFilePath_;
//};
//
//#endif // DATAPERSISTENCE_H


// File: src/DataPersistence.h

#ifndef DATAPERSISTENCE_H
#define DATAPERSISTENCE_H

#include "Portfolio.h"
#include "Order.h"
#include "ProfitLoss.h" // Newly added
#include <string>
#include <vector>

class DataPersistence {
public:
    // Constructor with file paths
    DataPersistence(const std::string& portfolioFile, const std::string& ordersFile, const std::string& plFile = "data/pl.json");

    // Portfolio methods
    bool loadPortfolio(Portfolio& portfolio);
    bool savePortfolio(const Portfolio& portfolio);

    // Orders methods
    bool loadOrders(std::vector<Order>& orders);
    bool saveOrders(const std::vector<Order>& orders);

    // Profit & Loss methods (newly added)
    bool loadProfitLoss(std::vector<ProfitLoss>& plList);
    bool saveProfitLoss(const std::vector<ProfitLoss>& plList);

private:
    std::string portfolioFilePath_;
    std::string ordersFilePath_;
    std::string plFilePath_; // Newly added
};

#endif // DATAPERSISTENCE_H