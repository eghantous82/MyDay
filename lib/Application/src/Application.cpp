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

    renderGoogleTasks(display);
    renderMarketInfo(display);
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
            if (i != 0) {
                oss << " GB: " << std::fixed << std::setprecision(1) << team.GamesBack;
            }
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
        display.printString(oss.str().c_str());
        display.setCursor(5, display.getCursorY() + 20);
        isFirst = false;
    }
}

void Application::renderMarketInfo(Adafruit_GFX& display) {
    std::vector<MarketApi::EquityInfo> equities;
    getMarketInfo(equities);
    display.setCursor(WIDTH/2+5, FONT_HEIGHT);
    for (const auto& equity : equities) {
        std::ostringstream oss;
        oss << equity.Name << ": " << (int)equity.Value << " (" << (int)equity.DayChange << ")";
        display.printString(oss.str().c_str());
        display.setCursor(WIDTH/2+5, display.getCursorY() + 20);
    }
}

void Application::renderBlynkInfo(Adafruit_GFX& display) {
    std::string blynkValue = getBlynkValue();
    display.setCursor(WIDTH/2 + 5,  HEIGHT/2 + FONT_HEIGHT);
    display.printString(std::string("Freezer Temp: " + blynkValue).c_str());
}

void Application::renderGoogleTasks(Adafruit_GFX& display) {
    std::vector<GoogleScriptApi::Task> tasks;
    getGoogleTasks(tasks);
    display.setCursor(5, FONT_HEIGHT);
    for (const auto& task : tasks) {
        display.printString(task.title.c_str());
        display.setCursor(5, display.getCursorY() + 20);
    }
}

void Application::getGoogleTasks(std::vector<GoogleScriptApi::Task>& tasks)
{
    GoogleScriptApi api(_secrets.getGoogleTasksUrl());
    tasks = api.getTasks(_retriever);
}

void Application::getMarketInfo(std::vector<MarketApi::EquityInfo>& equities)
{
    std::map<std::string, std::string> stockSymbols = {
        {"^IXIC", "NASDAQ"},
        {"^GSPTSE", "TSX"},
        {"^GSPC", "S&P 500"},
        {"^DJI", "DOW"},
        {"VGRO.TO", "VGRO"},
        {"TFII.TO", "TFII"},
        {"BEPC.TO", "BEPC"}
    };
    MarketApi marketApi(_secrets.getMarketApiKey());
    marketApi.getEquityInfo(stockSymbols, _retriever, equities);
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