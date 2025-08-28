#include <curl/curl.h>
#include <iostream>
#include "MockDataRetriever.h"


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string MockDataRetriever::getMlbData(const std::string& url)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string apiKey = "XXXX";
    
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
