#include "MockEpaperDisplay.h"
#include <iostream>
#include <curl/curl.h>


// Define colors to match GxEPD2
#define GxEPD_BLACK 0xFFFF
#define GxEPD_WHITE 0x0000

int WIDTH = 800;
int HEIGHT = 480;

void printString(MockEpaperDisplay& display, const std::string& str) {
    for (char c : str) {
        display.write(c);
    }
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

void getFootballScores()
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string season = "2025"; // Current season
    std::string url = "https://api.sportsdata.io/v3/mlb/scores/json/Standings/" + season;
    std::string apiKey = "8f68ac5a38504cd9aa868e01f7bb7952"; // Replace with your actual key

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
            std::cout << "Response:\n" << readBuffer << std::endl;
        } else {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }
    }

}

int main(int argc, char **argv) {

    // Create an instance of the mock display with the specified dimensions.
    MockEpaperDisplay display(WIDTH, HEIGHT);

    // Set the text color to BLACK so it's visible on the white background
    display.setTextColor(GxEPD_BLACK);

    // Fill the screen with white
    display.fillScreen(GxEPD_WHITE);

    // Draw a black rectangle tasks
    display.drawRect(5, 5, 395, 235, GxEPD_BLACK);

    // Draw a black rectangle sports scores
    display.drawRect(5, 240, 395, 235, GxEPD_BLACK);

    // Set the cursor position for the text
    display.setCursor(15, 15);
    printString(display, "Google tasks");
 
    // Set the cursor position for the text
    display.setCursor(15, 245);
    printString(display, "Sports scores");

    
    // Save the display's contents to a file
    display.saveToFile("epaper_output.png");

    getFootballScores();
    
    return 0;
}
