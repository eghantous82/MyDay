#pragma once
#include "IDataRetriever.h"
#include <vector>
#include <map>
#include <string>

class MarketApi {
public:
    struct EquityInfo {
        std::string Name;
        float Value;
        float DayChange;
        float FiftyTwoWeekChangePercentage;
    };

    MarketApi(const std::string& apiKey = "") : _apiKey(apiKey) {}

    void getEquityInfo(
        std::map<std::string, std::string>& equitiesMap,
        IDataRetriever& retriever,
        std::vector<EquityInfo>& equities);
private:
    std::string _apiKey;    
};