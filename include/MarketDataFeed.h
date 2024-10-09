//
// Created by jack on 9/10/24.
//

#ifndef MARKETDATAFEED_H
#define MARKETDATAFEED_H

#include <unordered_map>
#include <string>
#include <vector>

class MarketDataFeed {
public:
    MarketDataFeed();

    void updatePrices();
    double getPrice(const std::string& symbol) const;
    const std::unordered_map<std::string, double>& getPrices() const;

private:
    std::unordered_map<std::string, double> marketPrices_;
    std::vector<std::string> symbols_;

    void simulatePriceChanges();
};

#endif // MARKETDATAFEED_H