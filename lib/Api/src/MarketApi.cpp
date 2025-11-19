#include <ArduinoJson.h>
#include <sstream>
#include <iomanip>
#include "MarketApi.h"

void MarketApi::getEquityInfo(
    const std::map<std::string, std::string>& equitiesMap,
    IDataRetriever& retriever,
    std::vector<EquityInfo>& equities)
{
    // RFC3986-compliant URL encoder using std::string
    auto urlEncode = [](const std::string& value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;
        for (char c : value) {
            if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
            } else {
                escaped << '%' << std::setw(2) << std::uppercase << int((unsigned char)c);
            }
        }
        return escaped.str();
    };

    if(equitiesMap.empty()) {
        equities.emplace_back(EquityInfo{"Error", 1.0, 1.0, 1.0});
        return;
    }
    std::ostringstream oss;
    for (auto it = equitiesMap.begin(); it != equitiesMap.end(); ++it) {
        if (it != equitiesMap.begin()) {
            oss << ",";
        }
        oss << urlEncode(it->first);
    }
    std::string url = "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/quotes?ticker=" + oss.str();
    retriever.logToGoogle("Retrieving url: " + url);
    std::string json = retriever.getStockInfo(url, _apiKey);
    if(json.empty()) {
        equities.emplace_back(EquityInfo{"Error", 2.0, 2.0, 2.0});
        return;
    }
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        
        equities.emplace_back(EquityInfo{error.c_str(), 0.0, 0.0, 0.0});
        return;
    }

    auto arr = doc["body"].as<JsonArray>();
    equities.reserve(arr.size());
    for (const JsonObject& item : arr) {
        const auto& symbol = item["symbol"];
        equities.emplace_back(EquityInfo{
            equitiesMap.at(symbol),
            item["regularMarketPrice"],
            item["regularMarketChange"],
            item["fiftyTwoWeekChangePercent"]
        });
    }
}