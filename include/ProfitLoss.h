//
// Created by jack on 1/11/24.
//

// File: src/ProfitLoss.h

#ifndef PROFITLOSS_H
#define PROFITLOSS_H

#include <string>
// For JSON parsing
#include "json.hpp"
using json = nlohmann::json;

// Structure to represent Profit and Loss for a sell order
struct ProfitLoss {
    std::string symbol;
    int quantity;
    double sell_price;
    double average_buy_price;
    double profit_loss;
    std::string timestamp; // ISO8601 formatted string

    // Equality operator for easy comparison
    bool operator==(const ProfitLoss& other) const {
        return (symbol == other.symbol) &&
               (quantity == other.quantity) &&
               (sell_price == other.sell_price) &&
               (average_buy_price == other.average_buy_price) &&
               (profit_loss == other.profit_loss) &&
               (timestamp == other.timestamp);
    }
};

// JSON serialization for ProfitLoss
inline void to_json(nlohmann::json& j, const ProfitLoss& pl) {
    j = nlohmann::json{
            {"symbol", pl.symbol},
            {"quantity", pl.quantity},
            {"sell_price", pl.sell_price},
            {"average_buy_price", pl.average_buy_price},
            {"profit_loss", pl.profit_loss},
            {"timestamp", pl.timestamp}
    };
}

inline void from_json(const nlohmann::json& j, ProfitLoss& pl) {
    j.at("symbol").get_to(pl.symbol);
    j.at("quantity").get_to(pl.quantity);
    j.at("sell_price").get_to(pl.sell_price);
    j.at("average_buy_price").get_to(pl.average_buy_price);
    j.at("profit_loss").get_to(pl.profit_loss);
    j.at("timestamp").get_to(pl.timestamp);
}

#endif // PROFITLOSS_H