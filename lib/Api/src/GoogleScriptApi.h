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

    GoogleScriptApi(const std::string& url): _url(url) {}

    // Parses the JSON and returns a vector of Tasks
    std::pair<std::vector<Task>, std::vector<StockInfo> > getTasksAndStocks(IDataRetriever& dataRetriever);

private:
    std::string _url;
};
