#include "GoogleScriptApi.h"
#include <string>
#include <vector>
#include <iostream>
#include <ArduinoJson.h>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::pair<std::vector<GoogleScriptApi::Task>, std::vector<GoogleScriptApi::StockInfo> > GoogleScriptApi::getTasksAndStocks(IDataRetriever& dataRetriever) {
    std::vector<Task> tasks;
    std::vector<StockInfo> stocks;

    std::string response = dataRetriever.getGoogleTasks(_url);
    Task emptyTask;
    emptyTask.title = "Task free since '93";
    StockInfo emptyStock;
    emptyStock.ticker = "Error";
    emptyStock.displayName = "No Stock Info";
    if (response.empty())
    {
        tasks.push_back(emptyTask);
        stocks.push_back(emptyStock);
        return std::make_pair(tasks, stocks);
    }
    dataRetriever.logToGoogle("Received Google Information: " + response);
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        Task jsonErrorTask;
        jsonErrorTask.title = "JSON Error parsing tasks";

        StockInfo jsonErrorStock;
        jsonErrorStock.ticker = "JSON Error";
        jsonErrorStock.displayName = "parsing stocks";
        tasks.push_back(jsonErrorTask);
        stocks.push_back(jsonErrorStock);
        return std::make_pair(tasks, stocks);
    }

    JsonArray tasksArray = doc["tasks"];
    for (JsonObject list : tasksArray) {
        for (JsonObject task : list["tasks"].as<JsonArray>()) {
            Task t;
            t.title = task["title"] | "";
            t.status = task["status"] | "";
            t.due = task["due"] | "";
            tasks.push_back(t);
        }
    }
    if (tasks.empty()) {
        tasks.push_back(emptyTask);
    }
    JsonArray stocksArray = doc["stocks"];
    for (JsonObject stock : stocksArray) {
        StockInfo s;
        s.ticker = stock["ticker"] | "";
        s.displayName = stock["displayName"] | "";
        stocks.push_back(s);
    }
    
    return std::make_pair(tasks, stocks);
}
