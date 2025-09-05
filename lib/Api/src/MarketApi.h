#include "IDataRetriever.h"
#include <vector>
#pragma once

class MarketApi {
public:
    struct EquityInfo {
        std::string Name;
        float Value;
        float DayChange;
    };

    MarketApi(const std::string& apiKey = "") : _apiKey(apiKey) {}

    void getEquityInfo(IDataRetriever& retriever,
        std::vector<EquityInfo>& equities);
private:
    std::string _apiKey;    
};