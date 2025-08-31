#include <string>
#pragma once
class IDataRetriever {
public:
    virtual ~IDataRetriever() {}
    virtual std::string getMlbData(const std::string& url, const std::string& apiKey) = 0;
    virtual std::string getStockInfo(const std::string& url, const std::string& apiKey) = 0;
};