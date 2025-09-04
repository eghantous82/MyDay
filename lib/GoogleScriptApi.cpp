#include "GoogleScriptApi.h"
#include <string>
#include <vector>
#include <curl/curl.h>
#include <iostream>
#include <ArduinoJson.h>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<GoogleScriptApi::Task> GoogleScriptApi::getTasks(IDataRetriever& dataRetriever) {
    std::vector<Task> tasks;
    std::string response = dataRetriever.getGoogleTasks(_url);
    if (response.empty()) return tasks;

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        std::cerr << "Failed to parse tasks JSON: " << error.c_str() << std::endl;
        return tasks;
    }

    for (JsonObject list : doc.as<JsonArray>()) {
        for (JsonObject task : list["tasks"].as<JsonArray>()) {
            Task t;
            t.title = task["title"] | "";
            t.status = task["status"] | "";
            t.due = task["due"] | "";
            tasks.push_back(t);
        }
    }
    return tasks;
}
