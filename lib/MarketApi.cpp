#include "MarketApi.h"
//std::string res = retriever.getEquityInfo("https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/quotes?ticker=%5EIXIC,%5EGSPTSE,%5EGSPC,%5EDJI");

void MarketApi::getEquityInfo(IDataRetriever& retriever,
    std::vector<EquityInfo>& equities) {

    std::string json = retriever.getStockInfo("https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/quotes?ticker=%5EIXIC,%5EGSPTSE,%5EGSPC,%5EDJI", _apiKey);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        return;
    }

    for (JsonObject item : doc["body"].as<JsonArray>()) {
        EquityInfo info;
        info.Name = item["shortName"] | "";
        info.Value = item["regularMarketPrice"];
        info.DayChange = item["regularMarketChange"];
        equities.push_back(info);
    }
}