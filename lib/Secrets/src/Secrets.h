#include <string>
#include <ArduinoJson.h>

#pragma once
class Secrets {
public:
    Secrets(const std::string& secretsFile = "secrets.txt");
    std::string getBlynkAuthToken();
    std::string getMlbApiKey();
    std::string getMarketApiKey();
    std::string getGoogleTasksUrl();

private:
    JsonDocument _doc;
};