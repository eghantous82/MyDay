#include <curl/curl.h>
#include <iostream>
#include "MockDataRetriever.h"


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string MockDataRetriever::getMlbData(const std::string& url, const std::string& apiKey)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Ocp-Apim-Subscription-Key: " + apiKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (res == CURLE_OK) {
            std::cout << "Response: Success" << std::endl;
        } else {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return readBuffer;
}

std::string MockDataRetriever::getStockInfo(const std::string& url, const std::string& apiKey)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "x-rapidapi-host: yahoo-finance15.p.rapidapi.com");
        headers = curl_slist_append(headers, ("x-rapidapi-key: " + apiKey).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (res == CURLE_OK) {
            std::cout << "Response: Success" << std::endl;
        } else {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return readBuffer;
}