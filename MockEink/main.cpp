#include "MockEpaperDisplay.h"
#include "MockDataRetriever.h"

#include "Secrets/src/Secrets.h"

#include "Application/src/Application.h"
#include "Api/src/GoogleScriptApi.h"

int WIDTH = 800;
int HEIGHT = 480;

const int DISPLAY_AREA_WIDTH = WIDTH / 2;  // 400
const int DISPLAY_AREA_HEIGHT = HEIGHT / 2; // 240

struct DisplayArea {
  int16_t x, y, w, h;            // Position and dimensions of the area
};

DisplayArea displayAreas[] = {
  // Area 1: Top left (0–399 x 0–239)
  {0, 0, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT},

  // Area 2: Top right (400–799 x 0–239)
  {DISPLAY_AREA_WIDTH, 0, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT},

  // Area 3: Bottom left (0–399 x 240–479)
  {0, DISPLAY_AREA_HEIGHT, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT},

  // Area 4: Bottom right (400–799 x 240–479)
  {DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT}
};

int main(int argc, char **argv) {

    // Create an instance of the mock display with the specified dimensions.
    MockEpaperDisplay display(WIDTH, HEIGHT);

    MockDataRetriever retriever;
    Secrets secrets("../../../secrets.json");

    Application app(retriever, secrets, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT);
      std::vector<GoogleScriptApi::StockInfo> stocksToRetrieve;

    for (int i = 0; i < 4; i++)
    {
        // Prepare to draw only if needed
        bool update = false;

        switch (i) {
        case 0:
            update = app.shouldUpdateGoogle();
            break;
        case 1:
            update = app.shouldUpdateMarket();
            break;
        case 2:
            update = app.shouldUpdateMlb();
            break;
        case 3:
            update = app.shouldUpdateBlynk();
            break;
        }

        // Only refresh if something was drawn
        if (update)
        {
            display.setCursor(displayAreas[i].x + 5, displayAreas[i].y + 20);
            switch (i) {
            case 0:
                app.renderGoogleInfo(display, stocksToRetrieve);
                break;
            case 1:
                app.renderMarketInfo(display, stocksToRetrieve);
                break;
            case 2:
                app.renderNhlInfo(display);
                break;
            case 3:
                app.renderBlynkInfo(display);
                break;
            }
        }
    }

    display.saveToFile("epaper_output.png");
    
    return 0;
}
