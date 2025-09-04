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

    GoogleScriptApi(const std::string& url) : _url(url){}

    // Parses the JSON and returns a vector of Tasks
    std::vector<Task> getTasks(IDataRetriever& dataRetriever);

private:
    std::string _url;
};
