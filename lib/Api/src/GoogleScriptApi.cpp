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

void GoogleScriptApi::getCalendarEvents(IDataRetriever& dataRetriever, const std::string& url, std::vector<Event>& events) {
    std::string response = dataRetriever.getGoogleCalendarEvents(url);
    if (response.empty()) {
        Event errorEvent;
        errorEvent._summary = "Error retrieving calendar events";
        events.push_back(errorEvent);
        return;
    }
    dataRetriever.logToGoogle("Received Calendar Information: " + response);
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        Event jsonErrorEvent;
        jsonErrorEvent._summary = "JSON Error parsing calendar events";
        events.push_back(jsonErrorEvent);
        return;
    }

    JsonArray jsonEvents = doc["events"].as<JsonArray>();
    for (JsonObject event : jsonEvents)
    {
        GoogleScriptApi::Event newEvent;
        newEvent._summary = event["title"].as<std::string>();
        newEvent._location = event["location"].as<std::string>();

        struct tm startTime, endTime;
        if (!parseDateTime(event["startDate"].as<std::string>(), startTime) ||
            !parseDateTime(event["endDate"].as<std::string>(), endTime)) {
        }
        else {
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%H:%M", &startTime);
            newEvent._startTime = std::string(buffer);
            strftime(buffer, sizeof(buffer), "%H:%M", &endTime);
            newEvent._endTime = std::string(buffer);
        }

        addEventToList(events, newEvent);
    }
    if (events.empty()) {
        Event noEvents;
        noEvents._summary = "No upcoming events found.";
        events.push_back(noEvents);
    }
}

bool compareByTimestamp(const GoogleScriptApi::Event &a, const GoogleScriptApi::Event &b) {
  return a._startTime < b._startTime;  // ISO 8601 strings naturally sort correctly
}

void GoogleScriptApi::addEventToList(std::vector<GoogleScriptApi::Event>& events, Event& newEvent)
{
  auto pos = std::lower_bound(events.begin(), events.end(), newEvent, compareByTimestamp);
  events.insert(pos, newEvent);
}

bool GoogleScriptApi::parseDateTime(const std::string& datetime, struct tm& outTm) 
{
    int year, month, day, hour, minute, second;
    if (sscanf(datetime.c_str(), "%d-%d-%d %d:%d:%d",
               &year, &month, &day, &hour, &minute, &second) != 6) {
        return false; // Parsing failed
    }

    outTm = {};
    outTm.tm_year = year - 1900;
    outTm.tm_mon = month - 1;
    outTm.tm_mday = day;
    outTm.tm_hour = hour;
    outTm.tm_min = minute;
    outTm.tm_sec = second;
    outTm.tm_isdst = -1; // Let mktime determine DST

    return true;
}
