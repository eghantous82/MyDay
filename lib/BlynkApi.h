#pragma once
#include <string>

class BlynkApi {
public:

    BlynkApi(const std::string& token = "") : _token(token) {}

    // Retrieves the value of a virtual pin from Blynk Cloud using HTTP API
    // Returns empty string on failure
    std::string getVirtualPinValue(const std::string& pin);

private:
    std::string _token;
};
