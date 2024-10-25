////
//// Created by jack on 23/10/24.
////
//
//// File: src/DataPersistence.cpp
//
//// For JSON parsing
//#include "json.hpp"
//using json = nlohmann::json;
//
//namespace fs = std::filesystem;
//
//#include "DataPersistence.h"
//#include "Utils.h"      // For timePointToString and potentially parsing timestamps
//#include <fstream>
//#include <iostream>
//#include <sstream>
//#include <filesystem>   // The C++17 since filesystem library
//#include <iomanip>      // For std::setw
//
//
//// Constructor
//DataPersistence::DataPersistence(const std::string& portfolioFilePath, const std::string& ordersFilePath)
//        : portfolioFilePath_(portfolioFilePath),
//          ordersFilePath_(ordersFilePath) {}
//
//// Save Portfolio to JSON file
//bool DataPersistence::savePortfolio(const Portfolio& portfolio) const {
//    json j;
//    j["cashBalance"] = portfolio.getCashBalance();
//    j["peakValue"] = portfolio.getPeakValue();
//    j["drawdown"] = portfolio.getDrawdown();
//
//    // Serialize positions
//    j["positions"] = json::array();
//    for (const auto& [symbol, pos] : portfolio.getPositions()) {
//        json posJson;
//        posJson["symbol"] = symbol;
//        posJson["quantity"] = pos.quantity;
//        posJson["averagePrice"] = pos.averagePrice;
//        j["positions"].push_back(posJson);
//    }
//
//    // Serialize trade history
//    j["tradeHistory"] = json::array();
//    for (const auto& trade : portfolio.getTradeHistory()) {
//        j["tradeHistory"].push_back(trade.getOrder()); // Assumes Order has to_json implemented
//    }
//
//    // Serialize pending orders
//    j["pendingOrders"] = json::array();
//    for (const auto& order : portfolio.getPendingOrders()) {
//        j["pendingOrders"].push_back(order); // Assumes Order has to_json implemented
//    }
//
//    // Write to file
//    std::ofstream outFile(portfolioFilePath_);
//    if (!outFile.is_open()) {
//        std::cerr << "Failed to open portfolio file for writing: " << portfolioFilePath_ << "\n";
//        return false;
//    }
//
//    outFile << j.dump(4); // Pretty print with 4 spaces indentation
//    outFile.close();
//    return true;
//}
//
//// Load Portfolio from JSON file
//bool DataPersistence::loadPortfolio(Portfolio& portfolio) const {
//    std::ifstream inFile(portfolioFilePath_);
//    if (!inFile.is_open()) {
//        std::cerr << "Failed to open portfolio file for reading: " << portfolioFilePath_ << "\n";
//        return false;
//    }
//
//    json j;
//    try {
//        inFile >> j;
//    }
//    catch (const json::parse_error& e) {
//        std::cerr << "JSON parse error in portfolio file: " << e.what() << "\n";
//        return false;
//    }
//
//    // Deserialize cash balance, peak value, drawdown
//    double cashBalance = j.value("cashBalance", 0.0);
//    double peakValue = j.value("peakValue", 0.0);
//    double drawdown = j.value("drawdown", 0.0);
//    portfolio = Portfolio(cashBalance); // Re-initialize with cash balance
//    portfolio.setPeakValue(peakValue);
//    // Note: drawdown_ is updated based on portfolio's state, no direct setter
//
//// Deserialize positions
//    if (j.contains("positions")) {
//        for (const auto& posJson : j["positions"]) {
//            std::string symbol = posJson.value("symbol", "");
//            int quantity = posJson.value("quantity", 0);
//            double averagePrice = posJson.value("averagePrice", 0.0);
//            if (!symbol.empty() && quantity > 0 && averagePrice > 0.0) {
//                portfolio.updatePosition(symbol, quantity, averagePrice);
//            }
//        }
//    }
//
//    // Deserialize trade history
//    if (j.contains("tradeHistory")) {
//        for (const auto& tradeJson : j["tradeHistory"]) {
//            Order order = tradeJson.get<Order>(); // Assumes from_json implemented
//            Trade trade(order);
//            portfolio.addTrade(trade);
//        }
//    }
//
//    // Deserialize pending orders
//    if (j.contains("pendingOrders")) {
//        for (const auto& orderJson : j["pendingOrders"]) {
//            Order order = orderJson.get<Order>(); // Assumes from_json implemented
//            portfolio.addPendingOrder(order);
//        }
//    }
//
//    inFile.close();
//    return true;
//}
//
//// Save Orders to JSON file
//bool DataPersistence::saveOrders(const std::vector<Order>& orders) const {
//    json j = json::array();
//    for (const auto& order : orders) {
//        j.push_back(order); // Assumes Order has to_json implemented
//    }
//
//    // Write to file
//    std::ofstream outFile(ordersFilePath_);
//    if (!outFile.is_open()) {
//        std::cerr << "Failed to open orders file for writing: " << ordersFilePath_ << "\n";
//        return false;
//    }
//
//    outFile << j.dump(4); // Pretty print with 4 spaces indentation
//    outFile.close();
//    return true;
//}
//
//// Load Orders from JSON file
//bool DataPersistence::loadOrders(std::vector<Order>& orders) const {
//    std::ifstream inFile(ordersFilePath_);
//    if (!inFile.is_open()) {
//        std::cerr << "Failed to open orders file for reading: " << ordersFilePath_ << "\n";
//        return false;
//    }
//
//    json j;
//    try {
//        inFile >> j;
//    }
//    catch (const json::parse_error& e) {
//        std::cerr << "JSON parse error in orders file: " << e.what() << "\n";
//        return false;
//    }
//
//    for (const auto& orderJson : j) {
//        Order order = orderJson.get<Order>(); // Assumes from_json implemented
//        orders.push_back(order);
//    }
//
//    inFile.close();
//    return true;
//}


// File: src/DataPersistence.cpp

#include "DataPersistence.h"
#include "Utils.h"      // For timePointToString and potentially parsing timestamps
#include "json.hpp"     // nlohmann/json library
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>   // C++17 filesystem library
#include <iomanip>      // For std::setw

namespace fs = std::filesystem;
using json = nlohmann::json;

// Constructor
DataPersistence::DataPersistence(const std::string& portfolioFilePath, const std::string& ordersFilePath)
        : portfolioFilePath_(portfolioFilePath), ordersFilePath_(ordersFilePath) {
    // Ensure that the data directory exists
    fs::path portfolioPath(portfolioFilePath_);
    fs::path ordersPath(ordersFilePath_);
    fs::path dataDir = portfolioPath.parent_path();

    if (!fs::exists(dataDir)) {
        try {
            fs::create_directories(dataDir);
            std::cout << "Created data directory at: " << dataDir << "\n";
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error creating data directory: " << e.what() << "\n";
            throw; // Re-throw exception after logging
        }
    }
}

// Load Portfolio from JSON file
bool DataPersistence::loadPortfolio(Portfolio& portfolio) const {
    std::ifstream inFile(portfolioFilePath_);
    if (!inFile.is_open()) {
        std::cout << "Portfolio file not found. Initializing with default values.\n";
        return false; // Indicate that loading failed
    }

    json j;
    try {
        inFile >> j;
        portfolio.setCashBalance(j.at("cash_balance").get<double>());
        portfolio.setPeakValue(j.at("peak_value").get<double>());
        portfolio.setDrawdown(j.at("current_drawdown").get<double>());

        const auto& positionsJson = j.at("positions");
        for (auto it = positionsJson.begin(); it != positionsJson.end(); ++it) {
            std::string symbol = it.key();
            int quantity = it.value().at("quantity").get<int>();
            double averagePrice = it.value().at("average_price").get<double>();
            portfolio.addPosition(symbol, quantity, averagePrice);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error parsing portfolio JSON: " << e.what() << "\n";
        return false;
    }

    inFile.close();
    return true;
}

// Save Portfolio to JSON file
bool DataPersistence::savePortfolio(const Portfolio& portfolio) const {
    json j;
    j["cash_balance"] = portfolio.getCashBalance();
    j["peak_value"] = portfolio.getPeakValue();
    j["current_drawdown"] = portfolio.getDrawdown();

    json positionsJson;
    const auto& positions = portfolio.getPositions();
    for (const auto& [symbol, position] : positions) {
        positionsJson[symbol] = {
                {"quantity", position.quantity},
                {"average_price", position.averagePrice}
        };
    }
    j["positions"] = positionsJson;

    std::ofstream outFile(portfolioFilePath_);
    if (!outFile.is_open()) {
        std::cout << "Failed to open portfolio file for writing: " << portfolioFilePath_ << "\n";
        return false;
    }

    try {
        outFile << std::setw(4) << j << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error writing portfolio JSON: " << e.what() << "\n";
        return false;
    }

    outFile.close();
    return true;
}

// Load Orders from JSON file
bool DataPersistence::loadOrders(std::vector<Order>& orders) const {
    std::ifstream inFile(ordersFilePath_);
    if (!inFile.is_open()) {
        std::cout << "Orders file not found. Starting with no pending orders.\n";
        return false; // Indicate that loading failed
    }

    json j;
    try {
        inFile >> j;
        for (const auto& orderJson : j) {
            OrderType type;
            std::string typeStr = orderJson.at("type").get<std::string>();
            if (typeStr == "Buy") {
                type = OrderType::Buy;
            }
            else if (typeStr == "Sell") {
                type = OrderType::Sell;
            }
            else {
                std::cout << "Unknown order type: " << typeStr << ". Skipping order.\n";
                continue; // Skip invalid orders
            }

            OrderStyle style;
            std::string styleStr = orderJson.at("style").get<std::string>();
            if (styleStr == "Market") {
                style = OrderStyle::Market;
            }
            else if (styleStr == "Limit") {
                style = OrderStyle::Limit;
            }
            else {
                std::cout << "Unknown order style: " << styleStr << ". Skipping order.\n";
                continue; // Skip invalid orders
            }

            std::string symbol = orderJson.at("symbol").get<std::string>();
            int quantity = orderJson.at("quantity").get<int>();
            double price = orderJson.at("price").get<double>();
            double stopLoss = orderJson.value("stop_loss", 0.0);
            double takeProfit = orderJson.value("take_profit", 0.0);
            std::string timestampStr = orderJson.at("timestamp").get<std::string>();

            // Parse timestamp string to time_point
            std::tm tm = {};
            std::istringstream ss(timestampStr);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            if (ss.fail()) {
                std::cout << "Failed to parse timestamp: " << timestampStr << ". Skipping order.\n";
                continue; // Skip orders with invalid timestamps
            }
            std::time_t tt = std::mktime(&tm);
            if (tt == -1) {
                std::cout << "Invalid time conversion for timestamp: " << timestampStr << ". Skipping order.\n";
                continue;
            }
            std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::from_time_t(tt);

            // Construct Order object
            Order order(type, style, symbol, quantity, price, stopLoss, takeProfit, timestamp);
            orders.emplace_back(order);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error parsing orders JSON: " << e.what() << "\n";
        return false;
    }

    inFile.close();
    return true;
}

// Save Orders to JSON file
bool DataPersistence::saveOrders(const std::vector<Order>& orders) const {
    json j = json::array();
    for (const auto& order : orders) {
        json orderJson;
        orderJson["type"] = (order.getType() == OrderType::Buy) ? "Buy" : "Sell";
        orderJson["style"] = (order.getStyle() == OrderStyle::Market) ? "Market" : "Limit";
        orderJson["symbol"] = order.getSymbol();
        orderJson["quantity"] = order.getQuantity();
        orderJson["price"] = order.getPrice();
        orderJson["stop_loss"] = order.getStopLoss();
        orderJson["take_profit"] = order.getTakeProfit();
        orderJson["timestamp"] = timePointToString(order.getTimestamp());
        j.push_back(orderJson);
    }

    std::ofstream outFile(ordersFilePath_);
    if (!outFile.is_open()) {
        std::cout << "Failed to open orders file for writing: " << ordersFilePath_ << "\n";
        return false;
    }

    try {
        outFile << std::setw(4) << j << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error writing orders JSON: " << e.what() << "\n";
        return false;
    }

    outFile.close();
    return true;
}