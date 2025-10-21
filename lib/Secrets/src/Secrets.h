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
    std::string getSportsIoApiKey();
    std::string getMarketApiKey();
    std::string getGoogleTasksUrl();
    std::string getLoggerUrl();

private:
    ArduinoJson::JsonDocument _doc;
};