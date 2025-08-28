#include <string>
#pragma once
class IDataRetriever {
public:
    virtual ~IDataRetriever() {}
    virtual std::string getMlbData(const std::string& url) = 0;
};