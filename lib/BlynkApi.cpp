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

std::string BlynkApi::getVirtualPinValue(IDataRetriever& retriever) {
    std::string url = "https://blynk.cloud/external/api/get?token=" + _token +"&V1";
    return retriever.getBlynkValue(url);
}
