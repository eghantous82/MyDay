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

    std::ostringstream oss;
    retriever.logToGoogle("TTG " + equitiesMap.size());
    for (auto it = equitiesMap.begin(); it != equitiesMap.end(); ++it) {
        if (it != equitiesMap.begin()) {
            oss << ",";
        }
        retriever.logToGoogle("Adding " + it->first);
        oss << urlEncode(it->first);
    }
    std::string url = "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/quotes?ticker=" + oss.str();
    retriever.logToGoogle("Retrieving url: " + url);
    std::string json = retriever.getStockInfo(url, _apiKey);
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        equities.emplace_back(EquityInfo{"Error", 0.0, 0.0, 0.0});
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