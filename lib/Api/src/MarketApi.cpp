#include <ArduinoJson.h>
#include <sstream>
#include <UrlEncode.h>
#include "MarketApi.h"

std::string tickerSymbolsToCsv(const std::map<std::string, std::string>& dataMap) {
    std::ostringstream oss;
    for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
        if (it != dataMap.begin()) {
            oss << ",";
        }
        String tmp = it->first.c_str();
        std::string encodedTicker = urlEncode(tmp).c_str();
        oss << encodedTicker;
    }
    return oss.str();
}

void MarketApi::getEquityInfo(
    std::map<std::string, std::string>& equitiesMap,
    IDataRetriever& retriever,
    std::vector<EquityInfo>& equities)
{
    std::string commaSeparatedTickers = tickerSymbolsToCsv(equitiesMap);
    std::string url = "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/quotes?ticker=" + commaSeparatedTickers;
    Serial.println(url.c_str());
    std::string json = retriever.getStockInfo(url, _apiKey);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        EquityInfo errorInfo;
        errorInfo.Name = "Error";
        errorInfo.Value = 0.0;
        errorInfo.DayChange = 0.0;
        equities.push_back(errorInfo);
        return;
    }

    for (JsonObject item : doc["body"].as<JsonArray>()) {
        EquityInfo info;
        info.Name = equitiesMap[item["symbol"]];
        info.Value = item["regularMarketPrice"];
        info.DayChange = item["regularMarketChange"];
        info.FiftyTwoWeekChangePercentage = item["fiftyTwoWeekChangePercent"];
        equities.push_back(info);
    }
}