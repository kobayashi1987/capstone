//
// Created by jack on 23/10/24.
//

// File: src/DataPersistence.cpp

// For JSON parsing
#include "json.hpp"
using json = nlohmann::json;


#include "DataPersistence.h"
#include <fstream>
#include <iostream>

// Constructor
DataPersistence::DataPersistence(const std::string& portfolioFilePath, const std::string& ordersFilePath)
        : portfolioFilePath_(portfolioFilePath),
          ordersFilePath_(ordersFilePath) {}

// Save Portfolio to JSON file
bool DataPersistence::savePortfolio(const Portfolio& portfolio) const {
    json j;
    j["cashBalance"] = portfolio.getCashBalance();
    j["peakValue"] = portfolio.getPeakValue();
    j["drawdown"] = portfolio.getDrawdown();

    // Serialize positions
    j["positions"] = json::array();
    for (const auto& [symbol, pos] : portfolio.getPositions()) {
        json posJson;
        posJson["symbol"] = symbol;
        posJson["quantity"] = pos.quantity;
        posJson["averagePrice"] = pos.averagePrice;
        j["positions"].push_back(posJson);
    }

    // Serialize trade history
    j["tradeHistory"] = json::array();
    for (const auto& trade : portfolio.getTradeHistory()) {
        j["tradeHistory"].push_back(trade.getOrder()); // Assumes Order has to_json implemented
    }

    // Serialize pending orders
    j["pendingOrders"] = json::array();
    for (const auto& order : portfolio.getPendingOrders()) {
        j["pendingOrders"].push_back(order); // Assumes Order has to_json implemented
    }

    // Write to file
    std::ofstream outFile(portfolioFilePath_);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open portfolio file for writing: " << portfolioFilePath_ << "\n";
        return false;
    }

    outFile << j.dump(4); // Pretty print with 4 spaces indentation
    outFile.close();
    return true;
}

// Load Portfolio from JSON file
bool DataPersistence::loadPortfolio(Portfolio& portfolio) const {
    std::ifstream inFile(portfolioFilePath_);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open portfolio file for reading: " << portfolioFilePath_ << "\n";
        return false;
    }

    json j;
    try {
        inFile >> j;
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON parse error in portfolio file: " << e.what() << "\n";
        return false;
    }

    // Deserialize cash balance, peak value, drawdown
    double cashBalance = j.value("cashBalance", 0.0);
    double peakValue = j.value("peakValue", 0.0);
    double drawdown = j.value("drawdown", 0.0);
    portfolio = Portfolio(cashBalance); // Re-initialize with cash balance
    portfolio.setPeakValue(peakValue);
    // Note: drawdown_ is updated based on portfolio's state, no direct setter

// Deserialize positions
    if (j.contains("positions")) {
        for (const auto& posJson : j["positions"]) {
            std::string symbol = posJson.value("symbol", "");
            int quantity = posJson.value("quantity", 0);
            double averagePrice = posJson.value("averagePrice", 0.0);
            if (!symbol.empty() && quantity > 0 && averagePrice > 0.0) {
                portfolio.updatePosition(symbol, quantity, averagePrice);
            }
        }
    }

    // Deserialize trade history
    if (j.contains("tradeHistory")) {
        for (const auto& tradeJson : j["tradeHistory"]) {
            Order order = tradeJson.get<Order>(); // Assumes from_json implemented
            Trade trade(order);
            portfolio.addTrade(trade);
        }
    }

    // Deserialize pending orders
    if (j.contains("pendingOrders")) {
        for (const auto& orderJson : j["pendingOrders"]) {
            Order order = orderJson.get<Order>(); // Assumes from_json implemented
            portfolio.addPendingOrder(order);
        }
    }

    inFile.close();
    return true;
}

// Save Orders to JSON file
bool DataPersistence::saveOrders(const std::vector<Order>& orders) const {
    json j = json::array();
    for (const auto& order : orders) {
        j.push_back(order); // Assumes Order has to_json implemented
    }

    // Write to file
    std::ofstream outFile(ordersFilePath_);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open orders file for writing: " << ordersFilePath_ << "\n";
        return false;
    }

    outFile << j.dump(4); // Pretty print with 4 spaces indentation
    outFile.close();
    return true;
}

// Load Orders from JSON file
bool DataPersistence::loadOrders(std::vector<Order>& orders) const {
    std::ifstream inFile(ordersFilePath_);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open orders file for reading: " << ordersFilePath_ << "\n";
        return false;
    }

    json j;
    try {
        inFile >> j;
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON parse error in orders file: " << e.what() << "\n";
        return false;
    }

    for (const auto& orderJson : j) {
        Order order = orderJson.get<Order>(); // Assumes from_json implemented
        orders.push_back(order);
    }

    inFile.close();
    return true;
}