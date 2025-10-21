#include "Esp32DataRetriever.h"
#include <string>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

std::string Esp32DataRetriever::getSportsData(const std::string& url, const std::string& apiKey) {
    HTTPClient http;
    std::string readBuffer;

    http.begin(url.c_str()); // Initialize HTTP request
    http.addHeader("Ocp-Apim-Subscription-Key", apiKey.c_str()); // Add API key header

    int httpCode = http.GET(); // Send GET request

    if (httpCode > 0) {
        String payload = http.getString();
        readBuffer = payload.c_str(); // Convert Arduino String to std::string
    } else {
        readBuffer = "";
    }

    http.end(); // Close connection
    return readBuffer;
}


std::string Esp32DataRetriever::getStockInfo(const std::string& url, const std::string& apiKey) {
    HTTPClient http;
    std::string readBuffer;

    http.begin(url.c_str()); // Initialize HTTP request

    // Add RapidAPI headers
    http.addHeader("x-rapidapi-host", "yahoo-finance15.p.rapidapi.com");
    http.addHeader("x-rapidapi-key", apiKey.c_str());

    int httpCode = http.GET(); // Send GET request

    if (httpCode > 0) {
        String payload = http.getString();
        readBuffer = payload.c_str(); // Convert Arduino String to std::string
    } else {
        readBuffer = "";
    }

    http.end(); // Close connection
    return readBuffer;
}


std::string Esp32DataRetriever::getBlynkValue(const std::string& url) {
    HTTPClient http;
    std::string response;

    http.begin(url.c_str()); // Initialize HTTP request
    int httpCode = http.GET(); // Send GET request

    if (httpCode > 0) {
        // Request succeeded
        String payload = http.getString();
        response = payload.c_str(); // Convert Arduino String to std::string
    } else {
        // Request failed
        response = std::string("Error: ") + std::string(http.errorToString(httpCode).c_str());
    }

    http.end(); // Close connection
    return response;
}

std::string Esp32DataRetriever::getGoogleTasks(const std::string& url) {
    HTTPClient http;
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    if (!http.begin(url.c_str())) {
      return R"({
        "title": "Error retrieving tasks",
        "status": "Who cares"
      })"; // Return default task on failure to connect
    }
    int returnCode = http.GET();
    String response = http.getString();  // Print full response body
    http.end();
    return response.c_str();
}

void Esp32DataRetriever::logToGoogle(const std::string& message) {
    HTTPClient http;
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    if (!http.begin(_logUrl.c_str())) {
      return; // Return if failure to connect
    }
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Accept", "*/*");
    String jsonMessage = String("{\"message\":\"") + String(message.c_str()) + String("\"}");
    int returnCode = http.POST(jsonMessage);
    http.end();
}