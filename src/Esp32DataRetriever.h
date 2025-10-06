#pragma once
#include "../lib/IDataRetriever.h"
#include <string>

class Esp32DataRetriever : public IDataRetriever {
public:
    Esp32DataRetriever(const std::string& logUrl) : _logUrl(logUrl){}
    virtual ~Esp32DataRetriever() = default;

    std::string getMlbData(const std::string& url, const std::string& apiKey) override;
    std::string getStockInfo(const std::string& url, const std::string& apiKey) override;
    std::string getBlynkValue(const std::string& url) override;
    std::string getGoogleTasks(const std::string& url) override;
    void logToGoogle(const std::string& message) override;

private:
    std::string _logUrl;
};
