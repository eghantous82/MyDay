#include "Application.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

const int WIDTH = 800;
const int HEIGHT = 480;
const int FONT_HEIGHT = 20;
#ifdef ARDUINO
#define printString print
#include <GxEPD2_BW.h>
#endif
void Application::renderScreen(Adafruit_GFX& display) {

    // Set the text color to BLACK so it's visible on the white background
    display.setTextColor(GxEPD_BLACK);

    // Fill the screen with white
    display.fillScreen(GxEPD_WHITE);

    display.drawLine(WIDTH/2, 0, WIDTH/2, HEIGHT, GxEPD_BLACK);
    display.drawLine(0, HEIGHT/2, WIDTH, HEIGHT/2, GxEPD_BLACK);

    renderGoogleInfo(display);
    renderMlbInfo(display);
    renderBlynkInfo(display);

    // Display the last update time in the bottom right corner
    display.setCursor(WIDTH - 100, HEIGHT - FONT_HEIGHT);
    time_t now = time(nullptr);
    struct tm* timeInfo = localtime(&now);

    char timeStr[16];
    strftime(timeStr, sizeof(timeStr), "%H:%M", timeInfo); // 24-hour format

    display.printString(timeStr);

}


void Application::renderMlbInfo(Adafruit_GFX& display) {
    std::vector<MlbApi::TeamStanding> alStandings, alEastStandings;
    getMlbInfo(alStandings, alEastStandings);
    display.setCursor(5, HEIGHT/2 + FONT_HEIGHT);
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
            display.setCursor(5, display.getCursorY() + 20);
        }
    }
    display.setCursor(5, display.getCursorY() + 40);
    bool isFirst = true;
    for (const auto& team : alEastStandings) {
        std::ostringstream oss;
        oss << std::left << std::setw(3) << team.Team << ": " << team.Wins << "-" << team.Losses;
        if (!isFirst) {
            oss << " GB: " << std::fixed << std::setprecision(1) << team.GamesBack;
        }
        oss << " GR: " << (162 - team.Wins - team.Losses);
        display.printString(oss.str().c_str());
        display.setCursor(5, display.getCursorY() + 20);
        isFirst = false;
    }
}

void Application::renderMarketInfo(Adafruit_GFX& display, std::vector<GoogleScriptApi::StockInfo>& stocksToRetrieve) {
    std::vector<MarketApi::EquityInfo> equities;
    getMarketInfo(stocksToRetrieve, equities);
    display.setCursor(WIDTH/2+5, FONT_HEIGHT);
    for (const auto& equity : equities) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << equity.Name << ": " << equity.Value << " (" << equity.DayChange << ")";
        display.printString(oss.str().c_str());
        display.setCursor(WIDTH/2+5, display.getCursorY() + 20);
    }
}

void Application::renderBlynkInfo(Adafruit_GFX& display) {
    std::string blynkValue = getBlynkValue();
    display.setCursor(WIDTH/2 + 5,  HEIGHT/2 + FONT_HEIGHT);
    display.printString(std::string("Freezer Temp: " + blynkValue).c_str());
}

void Application::renderGoogleInfo(Adafruit_GFX& display) {
    std::pair<std::vector<GoogleScriptApi::Task>, std::vector<GoogleScriptApi::StockInfo> > googleInfo;
    getGoogleInfo(googleInfo);
    display.setCursor(5, FONT_HEIGHT);
    for (const auto& task : googleInfo.first) {
        display.printString(task.title.c_str());
        display.setCursor(5, display.getCursorY() + 20);
    }

    renderMarketInfo(display, googleInfo.second);
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