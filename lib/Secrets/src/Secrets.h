#pragma once
#include <string>
#include <ArduinoJson.h>

class Secrets {
public:
    Secrets(const std::string& secretsFile = "secrets.txt");
    std::string getBlynkAuthToken();
    std::string getMlbApiKey();
    std::string getMarketApiKey();
    std::string getGoogleTasksUrl();

private:
    ArduinoJson::StaticJsonDocument<1024> _doc;
};