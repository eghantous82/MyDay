#include "BlynkApi.h"
#include <string>
#include <sstream>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <curl/curl.h>

// Helper function for libcurl to write response data to a std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string BlynkApi::getVirtualPinValue(const std::string& pin) {
    std::string url = "https://blynk.cloud/external/api/get?token=" + _token +"&" + pin;
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            response = "";
        }
        curl_easy_cleanup(curl);
    }
    return response;
}
