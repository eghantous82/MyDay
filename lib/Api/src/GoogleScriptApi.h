#pragma once
#include <string>
#include <vector>
#include "IDataRetriever.h"

class GoogleScriptApi {
public:

    struct Task {
        std::string title;
        std::string status;
        std::string due;
    };
    
    struct StockInfo {
        std::string ticker;
        std::string displayName;
    };

    struct Event{
        std::string _startTime;
        std::string _endTime;
        std::string _summary;
        std::string _location;
    };

    GoogleScriptApi(const std::string& url): _url(url) {}

    // Parses the JSON and returns a vector of Tasks
    std::pair<std::vector<Task>, std::vector<StockInfo> > getTasksAndStocks(IDataRetriever& dataRetriever);

    void getCalendarEvents(IDataRetriever& dataRetriever, const std::string& url, std::vector<Event>& events);

private:

    static bool parseDateTime(const std::string& datetime, struct tm& outTm); 

    void addEventToList(std::vector<GoogleScriptApi::Event>& events, Event& newEvent);

    std::string _url;
};
