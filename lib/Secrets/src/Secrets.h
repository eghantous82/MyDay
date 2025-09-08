#pragma once
#include <string>
#include <ArduinoJson.h>

class Secrets {
public:
#ifndef ARDUINO
    Secrets(const std::string& secretsFile = "secrets.txt");
#else
    Secrets(const std::string& jsonString);
#endif
    std::string getBlynkAuthToken();
    std::string getMlbApiKey();
    std::string getMarketApiKey();
    std::string getGoogleTasksUrl();

private:
    ArduinoJson::JsonDocument _doc;
};