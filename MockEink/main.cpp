#include "MockEpaperDisplay.h"
#include "MockDataRetriever.h"
#include "MlbApi.h"
#include <iostream>

// Define colors to match GxEPD2
#define GxEPD_BLACK 0xFFFF
#define GxEPD_WHITE 0x0000

int WIDTH = 800;
int HEIGHT = 480;

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
    display.printString("Google tasks");
 
    MockDataRetriever retriever;
    MlbApi api;
    std::vector<MlbApi::TeamStanding> divisionStandings;
    std::vector<MlbApi::TeamStanding> alDivisionLeaders;
    api.getStandings(retriever, "AL", "East", divisionStandings, alDivisionLeaders);

    std::cout << "AL East Standings:\n";
    for (const auto& team : divisionStandings) {
        std::cout << team.Team << ": "
                  << team.Wins << "W-"
                  << team.Losses << "L, "
                  << "GB: " << team.GamesBack << std::endl;
    }

    // Now print the teams to the display buffer
    display.setCursor(15, 245);
    for (const auto& team : alDivisionLeaders) {
        display.printString(team.Team + ": " + std::to_string(team.Wins) + "W-" + std::to_string(team.Losses));
        display.setCursor(15, display.getCursorY() + 20);
    }
    display.saveToFile("epaper_output.png");
    return 0;
}
