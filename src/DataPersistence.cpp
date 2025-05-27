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


void exportPricesToCSV(const std::string& symbol,
                       const std::vector<double>& prices,
                       const std::string& outPath)
{
    std::ofstream out(outPath);
    out << "price\n";
    for (double p : prices) {
        out << p << "\n";
    }
    out.close();
}

