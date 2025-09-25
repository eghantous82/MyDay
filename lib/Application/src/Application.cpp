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

bool Application::renderMlbInfo(Adafruit_GFX& display) {

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
    _lastMlbRunTime = time(NULL);
    std::vector<MlbApi::TeamStanding> alStandings, alEastStandings;
    getMlbInfo(alStandings, alEastStandings);
    int x = display.getCursorX();
    if (!alStandings.empty()) {
        int leaderWins = alStandings[0].Wins;
        int leaderLosses = alStandings[0].Losses;
        for (size_t i = 0; i < alStandings.size(); ++i) {
            auto& team = alStandings[i];
            team.GamesBack = ((leaderWins - team.Wins) + (team.Losses - leaderLosses)) / 2.0f;
            std::ostringstream oss;
            oss << std::left << std::setw(3) << team.Team << ": " << team.Wins << "-" << team.Losses;
            oss << " GR: " << (162 - team.Wins - team.Losses);
            display.printString(oss.str().c_str());
            display.setCursor(x, display.getCursorY() + 20);
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
        display.setCursor(x, display.getCursorY() + 20);
        isFirst = false;
    }

    return true;
}

bool Application::renderMarketInfo(Adafruit_GFX& display, int x, std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve) {
    std::vector<MarketApi::EquityInfo> equities;
    time_t currentTime = time(NULL);
    struct tm* timeinfo = localtime(&currentTime);
    if(_lastStockInfoRunTime != 0) {
        
        if (difftime(currentTime, _lastStockInfoRunTime) < 60 * 15) {
            return false; // Skip if last update was less than 15 minutes ago
        }
    
        if (timeinfo->tm_wday == 0 || timeinfo->tm_wday == 6) {
            return false; // Skip weekends
        }

        if (timeinfo->tm_hour < 9 || (timeinfo->tm_hour == 9 && timeinfo->tm_min < 30) ||
            timeinfo->tm_hour > 16) {
            return false;
        }
    }
    _lastStockInfoRunTime = time(NULL);
    Serial.println("Fetching market info...");
    while(equities.size() == 0)
    {
        getMarketInfo(stocksToRetrieve, equities);
        if(equities.size() == 0) {
            delay(100);
        }
        else
        {
            for (const auto& equity : equities) {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2);
                oss << equity.Name << ": " << equity.Value << " (" << equity.DayChange << ") (" << equity.FiftyTwoWeekChangePercentage << "%)";
                display.printString(oss.str().c_str());
                display.setCursor(x, display.getCursorY() + 20);
            }
        }
    }
    return true;
}

bool Application::renderBlynkInfo(Adafruit_GFX& display) {
    if(_lastBlynkRunTime != 0) {
        time_t currentTime = time(NULL);
        if (difftime(currentTime, _lastBlynkRunTime) < 60 * 15) {
            return false; // Skip if last update was less than 15 minutes ago
        }
    }
    _lastBlynkRunTime = time(NULL);
    Serial.println("Fetching Blynk info...");
    std::string blynkValue = getBlynkValue();
    display.setCursor(display.getCursorX(), display.getCursorY() + 20);
    display.printString(std::string("Freezer Temp: " + blynkValue).c_str());
    return true;
}

bool Application::renderGoogleInfo(Adafruit_GFX& display, std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve) {
    if(_lastGoogleTaskRunTime != 0) {
        time_t currentTime = time(NULL);
        if (difftime(currentTime, _lastGoogleTaskRunTime) < 60 * 10) {
            return false; // Skip if last update was less than 15 minutes ago
        }

    }
    _lastGoogleTaskRunTime = time(NULL);
    Serial.println("Fetching Google info...");
    std::pair<std::vector<GoogleScriptApi::Task>, std::vector<GoogleScriptApi::StockInfo> > googleInfo;
    getGoogleInfo(googleInfo);
    Serial.printf("Retrieved %d tasks and %d stocks\n", (int)googleInfo.first.size(), (int)googleInfo.second.size());
    display.setCursor(5, FONT_HEIGHT);
    for (const auto& task : googleInfo.first) {
        display.printString(task.title.c_str());
        display.setCursor(5, display.getCursorY() + 20);
    }

    stocksToRetrieve = googleInfo.second;
    return true;
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