////
//// Created by jack on 23/10/24.
////
//
//#include "DataPersistence.h"
//#include "Utils.h"      // For timePointToString and potentially parsing timestamps
//#include "json.hpp"     // nlohmann/json library
//#include <fstream>
//#include <iostream>
//#include <sstream>
//#include <filesystem>   // C++17 filesystem library
//#include <iomanip>      // For std::setw
//
//namespace fs = std::filesystem;
//using json = nlohmann::json;
//
//// Constructor
//DataPersistence::DataPersistence(const std::string& portfolioFilePath, const std::string& ordersFilePath)
//        : portfolioFilePath_(portfolioFilePath), ordersFilePath_(ordersFilePath) {
//    // Ensure that the data directory exists
//    fs::path portfolioPath(portfolioFilePath_);
//    fs::path ordersPath(ordersFilePath_);
//    fs::path dataDir = portfolioPath.parent_path();
//
//    if (!fs::exists(dataDir)) {
//        try {
//            fs::create_directories(dataDir);
//            std::cout << "Created data directory at: " << dataDir << "\n";
//        }
//        catch (const fs::filesystem_error& e) {
//            std::cerr << "Error creating data directory: " << e.what() << "\n";
//            throw; // Re-throw exception after logging
//        }
//    }
//}
//
//// Load Portfolio from JSON file
//bool DataPersistence::loadPortfolio(Portfolio& portfolio) const {
//    std::ifstream inFile(portfolioFilePath_);
//    if (!inFile.is_open()) {
//        std::cout << "Portfolio file not found. Initializing with default values.\n";
//        return false; // Indicate that loading failed
//    }
//
//    json j;
//    try {
//        inFile >> j;
//        portfolio.setCashBalance(j.at("cash_balance").get<double>());
//        portfolio.setPeakValue(j.at("peak_value").get<double>());
//        portfolio.setDrawdown(j.at("current_drawdown").get<double>());
//
//        const auto& positionsJson = j.at("positions");
//        for (auto it = positionsJson.begin(); it != positionsJson.end(); ++it) {
//            std::string symbol = it.key();
//            int quantity = it.value().at("quantity").get<int>();
//            double averagePrice = it.value().at("average_price").get<double>();
//            portfolio.addPosition(symbol, quantity, averagePrice);
//        }
//    }
//    catch (const std::exception& e) {
//        std::cout << "Error parsing portfolio JSON: " << e.what() << "\n";
//        return false;
//    }
//
//    inFile.close();
//    return true;
//}
//
//// Save Portfolio to JSON file
//bool DataPersistence::savePortfolio(const Portfolio& portfolio) const {
//    json j;
//    j["cash_balance"] = portfolio.getCashBalance();
//    j["peak_value"] = portfolio.getPeakValue();
//    j["current_drawdown"] = portfolio.getDrawdown();
//
//    json positionsJson;
//    const auto& positions = portfolio.getPositions();
//    for (const auto& [symbol, position] : positions) {
//        positionsJson[symbol] = {
//                {"quantity", position.quantity},
//                {"average_price", position.averagePrice}
//        };
//    }
//    j["positions"] = positionsJson;
//
//    std::ofstream outFile(portfolioFilePath_);
//    if (!outFile.is_open()) {
//        std::cout << "Failed to open portfolio file for writing: " << portfolioFilePath_ << "\n";
//        return false;
//    }
//
//    try {
//        outFile << std::setw(4) << j << std::endl;
//    }
//    catch (const std::exception& e) {
//        std::cout << "Error writing portfolio JSON: " << e.what() << "\n";
//        return false;
//    }
//
//    outFile.close();
//    return true;
//}
//
//// Load Orders from JSON file
//bool DataPersistence::loadOrders(std::vector<Order>& orders) const {
//    std::ifstream inFile(ordersFilePath_);
//    if (!inFile.is_open()) {
//        std::cout << "Orders file not found. Starting with no pending orders.\n";
//        return false; // Indicate that loading failed
//    }
//
//    json j;
//    try {
//        inFile >> j;
//        for (const auto& orderJson : j) {
//            OrderType type;
//            std::string typeStr = orderJson.at("type").get<std::string>();
//            if (typeStr == "Buy") {
//                type = OrderType::Buy;
//            }
//            else if (typeStr == "Sell") {
//                type = OrderType::Sell;
//            }
//            else {
//                std::cout << "Unknown order type: " << typeStr << ". Skipping order.\n";
//                continue; // Skip invalid orders
//            }
//
//            OrderStyle style;
//            std::string styleStr = orderJson.at("style").get<std::string>();
//            if (styleStr == "Market") {
//                style = OrderStyle::Market;
//            }
//            else if (styleStr == "Limit") {
//                style = OrderStyle::Limit;
//            }
//            else {
//                std::cout << "Unknown order style: " << styleStr << ". Skipping order.\n";
//                continue; // Skip invalid orders
//            }
//
//            std::string symbol = orderJson.at("symbol").get<std::string>();
//            int quantity = orderJson.at("quantity").get<int>();
//            double price = orderJson.at("price").get<double>();
//            double stopLoss = orderJson.value("stop_loss", 0.0);
//            double takeProfit = orderJson.value("take_profit", 0.0);
//            std::string timestampStr = orderJson.at("timestamp").get<std::string>();
//
//            // Parse timestamp string to time_point
//            std::tm tm = {};
//            std::istringstream ss(timestampStr);
//            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
//            if (ss.fail()) {
//                std::cout << "Failed to parse timestamp: " << timestampStr << ". Skipping order.\n";
//                continue; // Skip orders with invalid timestamps
//            }
//            std::time_t tt = std::mktime(&tm);
//            if (tt == -1) {
//                std::cout << "Invalid time conversion for timestamp: " << timestampStr << ". Skipping order.\n";
//                continue;
//            }
//            std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::from_time_t(tt);
//
//            // Construct Order object
//            Order order(type, style, symbol, quantity, price, stopLoss, takeProfit, timestamp);
//            orders.emplace_back(order);
//        }
//    }
//    catch (const std::exception& e) {
//        std::cout << "Error parsing orders JSON: " << e.what() << "\n";
//        return false;
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
//        json orderJson;
//        orderJson["type"] = (order.getType() == OrderType::Buy) ? "Buy" : "Sell";
//        orderJson["style"] = (order.getStyle() == OrderStyle::Market) ? "Market" : "Limit";
//        orderJson["symbol"] = order.getSymbol();
//        orderJson["quantity"] = order.getQuantity();
//        orderJson["price"] = order.getPrice();
//        orderJson["stop_loss"] = order.getStopLoss();
//        orderJson["take_profit"] = order.getTakeProfit();
//        orderJson["timestamp"] = timePointToString(order.getTimestamp());
//        j.push_back(orderJson);
//    }
//
//    std::ofstream outFile(ordersFilePath_);
//    if (!outFile.is_open()) {
//        std::cout << "Failed to open orders file for writing: " << ordersFilePath_ << "\n";
//        return false;
//    }
//
//    try {
//        outFile << std::setw(4) << j << std::endl;
//    }
//    catch (const std::exception& e) {
//        std::cout << "Error writing orders JSON: " << e.what() << "\n";
//        return false;
//    }
//
//    outFile.close();
//    return true;
//}


// File: src/DataPersistence.cpp

#include "DataPersistence.h"
#include "ProfitLoss.h"
#include "Order.h"
#include <fstream>
#include <iostream>

// For JSON parsing
#include "json.hpp"
using json = nlohmann::json;

// Constructor with file paths
DataPersistence::DataPersistence(const std::string& portfolioFile, const std::string& ordersFile, const std::string& plFile)
        : portfolioFilePath_(portfolioFile),
          ordersFilePath_(ordersFile),
          plFilePath_(plFile) {}

// Load Portfolio
bool DataPersistence::loadPortfolio(Portfolio& portfolio) {
    std::ifstream inFile(portfolioFilePath_);
    if (!inFile.is_open()) {
        std::cout << "Portfolio file not found. Starting with a new portfolio.\n";
        return false;
    }

    nlohmann::json j;
    inFile >> j;

    // Deserialize portfolio
    if (j.contains("cashBalance") && j.contains("peakValue") && j.contains("drawdown")) {
        portfolio.setCashBalance(j["cashBalance"].get<double>());
        portfolio.setPeakValue(j["peakValue"].get<double>());
        portfolio.setDrawdown(j["drawdown"].get<double>());
    }

    // Deserialize positions
    if (j.contains("positions")) {
        for (auto& [symbol, pos] : j["positions"].items()) {
            portfolio.addPosition(symbol, pos["quantity"].get<int>(), pos["averagePrice"].get<double>());
        }
    }

    // Deserialize P&L
    if (j.contains("profitLoss")) {
        std::vector<ProfitLoss> plList = j["profitLoss"].get<std::vector<ProfitLoss>>();
        portfolio.setProfitLossList(plList);
    }

    inFile.close();
    return true;
}

// Save Portfolio
bool DataPersistence::savePortfolio(const Portfolio& portfolio) {
    nlohmann::json j;
    j["cashBalance"] = portfolio.getCashBalance();
    j["peakValue"] = portfolio.getPeakValue();
    j["drawdown"] = portfolio.getDrawdown();

    // Serialize positions
    nlohmann::json positionsJson;
    for (const auto& [symbol, pos] : portfolio.getPositions()) {
        positionsJson[symbol] = {
                {"quantity", pos.quantity},
                {"averagePrice", pos.averagePrice}
        };
    }
    j["positions"] = positionsJson;

    // Serialize P&L
    j["profitLoss"] = portfolio.getProfitLossList();

    std::ofstream outFile(portfolioFilePath_);
    if (!outFile.is_open()) {
        std::cout << "Failed to open portfolio file for writing.\n";
        return false;
    }

    outFile << j.dump(4); // Pretty print with 4-space indentation
    outFile.close();
    return true;
}
bool DataPersistence::loadOrders(std::vector<Order>& orders) {
    std::ifstream inFile(ordersFilePath_);
    if (!inFile.is_open()) {
        std::cout << "Orders file not found at path: " << ordersFilePath_ << ". Starting with no pending orders.\n";
        return false;
    }

    nlohmann::json j;
    try {
        inFile >> j;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cout << "Error parsing orders file: " << e.what() << "\n";
        inFile.close();
        return false;
    }

    // Deserialize orders
    if (j.is_array()) {
        for (const auto& item : j) {
            // Pre-validate required fields and types
            bool valid = true;
            std::string typeStr, styleStr, symbol, timestamp;
            int quantity;
            double price, stopLoss, takeProfit;

            // Validate 'type'
            if (item.contains("type") && item["type"].is_string()) {
                typeStr = item["type"].get<std::string>();
                if (typeStr != "Buy" && typeStr != "Sell") {
                    std::cout << "Invalid 'type' value: " << typeStr << "\n";
                    valid = false;
                }
            } else {
                std::cout << "Missing or invalid 'type' field.\n";
                valid = false;
            }

            // Validate 'style'
            if (item.contains("style") && item["style"].is_string()) {
                styleStr = item["style"].get<std::string>();
                if (styleStr != "Market" && styleStr != "Limit") {
                    std::cout << "Invalid 'style' value: " << styleStr << "\n";
                    valid = false;
                }
            } else {
                std::cout << "Missing or invalid 'style' field.\n";
                valid = false;
            }

            // Validate 'symbol'
            if (item.contains("symbol") && item["symbol"].is_string()) {
                symbol = item["symbol"].get<std::string>();
            } else {
                std::cout << "Missing or invalid 'symbol' field.\n";
                valid = false;
            }

            // Validate 'quantity'
            if (item.contains("quantity") && item["quantity"].is_number_integer()) {
                quantity = item["quantity"].get<int>();
                if (quantity <= 0) {
                    std::cout << "Invalid 'quantity' value: " << quantity << ". Must be positive.\n";
                    valid = false;
                }
            } else {
                std::cout << "Missing or invalid 'quantity' field.\n";
                valid = false;
            }

            // Validate 'price'
            if (item.contains("price") && item["price"].is_number()) {
                price = item["price"].get<double>();
                if (price <= 0.0) {
                    std::cout << "Invalid 'price' value: " << price << ". Must be positive.\n";
                    valid = false;
                }
            } else {
                std::cout << "Missing or invalid 'price' field.\n";
                valid = false;
            }

            // Validate 'stopLoss'
            if (item.contains("stopLossPrice") && item["stopLossPrice"].is_number()) {
                stopLoss = item["stopLossPrice"].get<double>();
                if (stopLoss <= 0.0) {
                    std::cout << "Invalid 'stopLoss' value: " << stopLoss << ". Must be positive.\n";
                    valid = false;
                }
            } else {
                std::cout << "Missing or invalid 'stopLoss' field.\n";
                valid = false;
            }

            // Validate 'takeProfit'
            if (item.contains("takeProfitPrice") && item["takeProfitPrice"].is_number()) {
                takeProfit = item["takeProfitPrice"].get<double>();
                if (takeProfit <= 0.0) {
                    std::cout << "Invalid 'takeProfit' value: " << takeProfit << ". Must be positive.\n";
                    valid = false;
                }
            } else {
                std::cout << "Missing or invalid 'takeProfit' field.\n";
                valid = false;
            }

            // Validate 'timestamp'
            if (item.contains("timestamp") && item["timestamp"].is_string()) {
                timestamp = item["timestamp"].get<std::string>();
                // Optional: Validate timestamp format using regex or date parsing
            } else {
                std::cout << "Missing or invalid 'timestamp' field.\n";
                valid = false;
            }

            if (valid) {
                try {
                    Order order = item.get<Order>();
                    orders.push_back(order);
                }
                catch (const nlohmann::json::type_error& e) {
                    std::cout << "Type error deserializing an order: " << e.what() << "\n";
                    std::cout << "Faulty order data: " << item.dump() << "\n";
                }
                catch (const std::invalid_argument& e) {
                    std::cout << "Invalid argument error deserializing an order: " << e.what() << "\n";
                    std::cout << "Faulty order data: " << item.dump() << "\n";
                }
                catch (const std::exception& e) {
                    std::cout << "General error deserializing an order: " << e.what() << "\n";
                    std::cout << "Faulty order data: " << item.dump() << "\n";
                }
            }
            else {
                std::cout << "Skipping faulty order data: " << item.dump() << "\n";
            }
        }
    }
    else {
        std::cout << "Orders file is not a valid JSON array at path: " << ordersFilePath_ << ". Starting with no pending orders.\n";
        inFile.close();
        return false;
    }

    inFile.close();
    return true;
}

// Save Orders
bool DataPersistence::saveOrders(const std::vector<Order>& orders) {
    nlohmann::json j = orders;

    std::ofstream outFile(ordersFilePath_);
    if (!outFile.is_open()) {
        std::cout << "Failed to open orders file for writing.\n";
        return false;
    }

    outFile << j.dump(4); // Pretty print with 4-space indentation
    outFile.close();
    return true;
}

//// Load Profit & Loss
//bool DataPersistence::loadProfitLoss(std::vector<ProfitLoss>& plList) {
//    std::ifstream inFile(plFilePath_);
//    if (!inFile.is_open()) {
//        std::cout << "Profit & Loss file not found at path: " << plFilePath_ << ". Starting with no P&L records.\n";
//        return false;
//    }
//
//    nlohmann::json j;
//    try {
//        inFile >> j;
//    }
//    catch (const nlohmann::json::parse_error& e) {
//        std::cout << "Error parsing Profit & Loss file at path " << plFilePath_ << ": " << e.what() << "\n";
//        inFile.close();
//        return false;
//    }
//
//    // Deserialize P&L entries
//    if (j.is_array()) {
//        try {
//            plList = j.get<std::vector<ProfitLoss>>();
//            // Optionally, validate each pl entry
//            std::vector<ProfitLoss> validPlList;
//            for (const auto& pl : plList) {
//                if (!pl.symbol.empty() && pl.quantity > 0 && pl.sell_price > 0 &&
//                    pl.average_buy_price > 0 && !pl.timestamp.empty()) {
//                    validPlList.push_back(pl);
//                }
//                else {
//                    std::cout << "Invalid ProfitLoss entry detected in pl.json. Skipping: " << nlohmann::json(pl).dump() << "\n";
//                }
//            }
//            plList = validPlList;
//        }
//        catch (const nlohmann::json::type_error& e) {
//            std::cout << "Error deserializing Profit & Loss entries from " << plFilePath_ << ": " << e.what() << "\n";
//            inFile.close();
//            return false;
//        }
//    }
//    else {
//        std::cout << "Profit & Loss file at path " << plFilePath_ << " is not a valid JSON array. Starting with no P&L records.\n";
//        inFile.close();
//        return false;
//    }
//
//    inFile.close();
//    std::cout << "Profit & Loss records loaded successfully from " << plFilePath_ << ".\n";
//    return true;
//}

// Load Profit & Loss
bool DataPersistence::loadProfitLoss(std::vector<ProfitLoss>& plList) {
    std::ifstream inFile(plFilePath_);
    if (!inFile.is_open()) {
        std::cout << "Profit & Loss file not found at path: " << plFilePath_ << ". Creating a new pl.json file with an empty array.\n";

        // Create pl.json with empty array
        std::ofstream outFile(plFilePath_);
        if (!outFile.is_open()) {
            std::cout << "Failed to create Profit & Loss file at path: " << plFilePath_ << ". Starting with no P&L records.\n";
            return false;
        }
        outFile << "[]";
        outFile.close();
        std::cout << "Initialized pl.json with an empty array.\n";
        return false;
    }

    nlohmann::json j;
    try {
        inFile >> j;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cout << "Error parsing Profit & Loss file at path " << plFilePath_ << ": " << e.what() << "\n";
        inFile.close();
        return false;
    }

    // Deserialize P&L entries
    if (j.is_array()) {
        try {
            plList = j.get<std::vector<ProfitLoss>>();
            // Optionally, validate each pl entry
            std::vector<ProfitLoss> validPlList;
            for (const auto& pl : plList) {
                if (!pl.symbol.empty() && pl.quantity > 0 && pl.sell_price > 0 &&
                    pl.average_buy_price > 0 && !pl.timestamp.empty()) {
                    validPlList.push_back(pl);
                }
                else {
                    std::cout << "Invalid ProfitLoss entry detected in pl.json. Skipping: " << nlohmann::json(pl).dump() << "\n";
                }
            }
            plList = validPlList;
        }
        catch (const nlohmann::json::type_error& e) {
            std::cout << "Error deserializing Profit & Loss entries from " << plFilePath_ << ": " << e.what() << "\n";
            inFile.close();
            return false;
        }
    }
    else {
        std::cout << "Profit & Loss file at path " << plFilePath_ << " is not a valid JSON array. Starting with no P&L records.\n";
        inFile.close();
        return false;
    }

    inFile.close();
    std::cout << "Profit & Loss records loaded successfully from " << plFilePath_ << ".\n";
    return true;
}

// Save Profit & Loss
bool DataPersistence::saveProfitLoss(const std::vector<ProfitLoss>& plList) {
    nlohmann::json j = plList;

    std::ofstream outFile(plFilePath_);
    if (!outFile.is_open()) {
        std::cout << "Failed to open Profit & Loss file for writing.\n";
        return false;
    }

    outFile << j.dump(4); // Pretty print with 4-space indentation
    outFile.close();
    return true;
}


