#include "Application.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

const int FONT_HEIGHT = 20;
#ifdef ARDUINO
#define printString print
#include <GxEPD2_BW.h>
#endif

bool Application::shouldUpdateMlb() {
    time_t currentTime = time(NULL);
    struct tm* timeinfo = localtime(&currentTime);
    if(_lastMlbRunTime != 0) {
        if (timeinfo->tm_hour < 7) {
            return false;
        }

        if (difftime(currentTime, _lastMlbRunTime) < 60 * 60 * 4) {
            return false; // Skip if last update was less than 4 hours ago
        }
    }
    return true;
}

bool Application::shouldUpdateMarket() {
    time_t currentTime = time(NULL);
    struct tm* timeinfo = localtime(&currentTime);
    if(_lastStockInfoRunTime != 0) {
        
        if (difftime(currentTime, _lastStockInfoRunTime) < 60 * 90) {
            return false; // Skip if last update was less than 90 minutes ago
        }
    
        if (timeinfo->tm_wday == 0 || timeinfo->tm_wday == 6) {
            return false; // Skip weekends
        }

        if (timeinfo->tm_hour < 9 || (timeinfo->tm_hour == 9 && timeinfo->tm_min < 30) ||
            timeinfo->tm_hour > 17) {
            return false;
        }
    }
    return true;
}

bool Application::shouldUpdateBlynk() {
    time_t currentTime = time(NULL);
    if(_lastBlynkRunTime != 0) {
        if (difftime(currentTime, _lastBlynkRunTime) < 60 * 15) {
            return false; // Skip if last update was less than 15 minutes ago
        }
    }
    return true;
}

bool Application::shouldUpdateGoogle() {
    time_t currentTime = time(NULL);
    if(_lastGoogleTaskRunTime != 0) {
        if (difftime(currentTime, _lastGoogleTaskRunTime) < 60 * 10) {
            return false; // Skip if last update was less than 10 minutes ago
        }
    }
    return true;
}

void Application::renderMlbInfo(Adafruit_GFX& display) {

    _lastMlbRunTime = time(NULL);
    std::vector<MlbApi::TeamStanding> alStandings, alEastStandings;
    getMlbInfo(alStandings, alEastStandings);
    alStandings.reserve(10); // Typical division size
    alEastStandings.reserve(10);
    int x = display.getCursorX();
    int y = display.getCursorY() - FONT_HEIGHT;
    if (!alStandings.empty()) {
        int leaderWins = alStandings[0].Wins;
        int leaderLosses = alStandings[0].Losses;
        for (auto& team : alStandings) {
            team.GamesBack = ((leaderWins - team.Wins) + (team.Losses - leaderLosses)) / 2.0f;
            std::ostringstream oss;
            oss << std::left << std::setw(3) << team.Team << ": " << team.Wins << "-" << team.Losses;
            oss << " GR: " << (162 - team.Wins - team.Losses);
            display.printString(oss.str().c_str());
            display.setCursor(x, display.getCursorY() + FONT_HEIGHT);
        }
    }
    display.setCursor(x, display.getCursorY() + 40);
    bool isFirst = true;
    for (const auto& team : alEastStandings) {
        std::ostringstream oss;
        oss << std::left << std::setw(3) << team.Team << ": " << team.Wins << "-" << team.Losses;
        if (!isFirst) {
            oss << " GB: " << std::fixed << std::setprecision(1) << team.GamesBack;
        }
        oss << " GR: " << (162 - team.Wins - team.Losses);
        display.printString(oss.str().c_str());
        display.setCursor(x, display.getCursorY() + FONT_HEIGHT);
        isFirst = false;
    }

    display.setCursor(x + _quadrantWidth - 80, y + _quadrantHeight - FONT_HEIGHT);
    // Display last updated time    
    struct tm* timeinfo = localtime(&_lastMlbRunTime);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);
    display.printString(buffer);


}

void Application::renderMarketInfo(Adafruit_GFX& display, std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve) {
    std::vector<MarketApi::EquityInfo> equities;
    equities.reserve(stocksToRetrieve.size());
    int x = display.getCursorX();
    int y = display.getCursorY() - FONT_HEIGHT;
    _lastStockInfoRunTime = time(NULL);
    getMarketInfo(stocksToRetrieve, equities);
    for (const auto& equity : equities) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << equity.Name << ": " << equity.Value << " (" << equity.DayChange << ") (" << equity.FiftyTwoWeekChangePercentage << "%)";
        display.printString(oss.str().c_str());
        display.setCursor(x, display.getCursorY() +  FONT_HEIGHT);
    }

    // Display last updated time
    display.setCursor(x + _quadrantWidth - 80, y + _quadrantHeight - FONT_HEIGHT);
    struct tm* timeinfo = localtime(&_lastStockInfoRunTime);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);
    display.printString(buffer);
}

void Application::renderBlynkInfo(Adafruit_GFX& display) {
    _lastBlynkRunTime = time(NULL);
    int x = display.getCursorX();
    int y = display.getCursorY() - FONT_HEIGHT;
    std::string blynkValue = getBlynkValue();
    display.setCursor(display.getCursorX(), display.getCursorY() + FONT_HEIGHT);
    display.printString(std::string("Freezer Temp: " + blynkValue).c_str());

    display.setCursor(x + _quadrantWidth - 80, y + _quadrantHeight - FONT_HEIGHT);
    // Display last updated time    
    struct tm* timeinfo = localtime(&_lastBlynkRunTime);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);
    display.printString(buffer);

}

void Application::renderGoogleInfo(Adafruit_GFX& display, std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve) {
    _lastGoogleTaskRunTime = time(NULL);
    int x = display.getCursorX();
    int y = display.getCursorY() - FONT_HEIGHT;
    std::pair<std::vector<GoogleScriptApi::Task>, std::vector<GoogleScriptApi::StockInfo> > googleInfo;
    getGoogleInfo(googleInfo);
    googleInfo.first.reserve(10);
    display.setCursor(x, FONT_HEIGHT);
    for (const auto& task : googleInfo.first) {
        display.printString(task.title.c_str());
        display.setCursor(x, display.getCursorY() + FONT_HEIGHT);
    }

    display.setCursor(x + _quadrantWidth - 80, y + _quadrantHeight - FONT_HEIGHT);
    // Display last updated time    
    struct tm* timeinfo = localtime(&_lastGoogleTaskRunTime);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);
    display.printString(buffer);

    stocksToRetrieve = googleInfo.second;
}

void Application::getGoogleInfo(std::pair<std::vector<GoogleScriptApi::Task>, std::vector<GoogleScriptApi::StockInfo> >& googleInfo)
{
    GoogleScriptApi api(_secrets.getGoogleTasksUrl());
    googleInfo = api.getTasksAndStocks(_retriever);
}

void Application::getMarketInfo(std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve, std::vector<MarketApi::EquityInfo>& equities)
{
    // Symbol to Name map
    std::map<std::string, std::string> symbolsToNames;
    for(const auto& stock : stocksToRetrieve) {
        symbolsToNames[stock.ticker] = stock.displayName;
    }
    MarketApi marketApi(_secrets.getMarketApiKey());
    marketApi.getEquityInfo(symbolsToNames, _retriever, equities);
}

void Application::getMlbInfo(std::vector<MlbApi::TeamStanding>& alStandings,
    std::vector<MlbApi::TeamStanding>& alEastStandings)
{
    MlbApi api(_secrets.getMlbApiKey());
    api.getStandings(_retriever, "AL", "East", alEastStandings, alStandings);
}

std::string Application::getBlynkValue()
{
    BlynkApi blynkApi(_secrets.getBlynkAuthToken());
    return blynkApi.getVirtualPinValue(_retriever);
}