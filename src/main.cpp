#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <WiFi.h>
#include <Preferences.h>
#include <time.h>
#include "Application.h"
#include "SecretTokens.h"
#include "Secrets.h"
#include "Esp32DataRetriever.h"

#define printString print
#include <GxEPD2_BW.h>

/**
 * @brief Represents a single display area with its update schedule.
 * This struct makes it easy to manage multiple distinct update regions.
 */
struct DisplayArea {
  int16_t x, y, w, h;            // Position and dimensions of the area
};

const int DISPLAY_AREA_WIDTH = 800 / 2;  // 400
const int DISPLAY_AREA_HEIGHT = 480 / 2; // 240
const int MEMORY_STATS_HEIGHT = 20;

DisplayArea displayAreas[] = {
  // Area 1: Top left (0–399 x 0–239)
  {0, 0, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT},

  // Area 2: Top right (400–799 x 0–239)
  {DISPLAY_AREA_WIDTH, 0, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT},

  // Area 3: Bottom left (0–399 x 240–479)
  {0, DISPLAY_AREA_HEIGHT, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT - MEMORY_STATS_HEIGHT},

  // Area 4: Bottom right (400–799 x 240–459)
  {DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT - MEMORY_STATS_HEIGHT},

   // Area 5: Bottom right (400–799 x 460–479)
  {0, 480-MEMORY_STATS_HEIGHT-15, 800, MEMORY_STATS_HEIGHT + 15},

};

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=*/3, /*DC=*/5, /*RST=*/2, /*BUSY=*/4));

Secrets secrets(SECRET_TOKENS);
Esp32DataRetriever retriever(secrets.getLoggerUrl());
Application app(retriever, secrets, DISPLAY_AREA_WIDTH, DISPLAY_AREA_HEIGHT);


void drawMemoryStats(Adafruit_GFX &display) {
  // Stack usage
  UBaseType_t watermarkWords = uxTaskGetStackHighWaterMark(NULL);
  size_t watermarkBytes = watermarkWords * sizeof(StackType_t);
  const size_t totalStackBytes = CONFIG_ARDUINO_LOOP_STACK_SIZE;
  size_t usedStackBytes = totalStackBytes - watermarkBytes;
  float stackPercent = (float)usedStackBytes / totalStackBytes * 100;
  Serial.println("Stack usage calculated");
  // Heap usage
  size_t freeHeap = ESP.getFreeHeap();
  size_t minFreeHeap = ESP.getMinFreeHeap();
  size_t maxAllocHeap = ESP.getMaxAllocHeap();

  Serial.println("Got all the stats");
  // Format output
  char buffer[128];
  snprintf(buffer, sizeof(buffer),
    "S: %u/%u (%.1f%%) | "
    "H:  %u fr | "
    "Min: %u | "
    "MaxBlk:%u",
    usedStackBytes, totalStackBytes, stackPercent,
    freeHeap,
    minFreeHeap,
    maxAllocHeap
  );

  Serial.println(buffer);
  // Display or log
  display.printString(buffer);
}


void syncTime() {
  // Configure NTP client with a server and wait for synchronization.
  // The first two parameters are for daylight saving time offset and standard time offset in seconds.
  // We are setting them to 0 as we will use the environment variable to set the timezone.
  configTime(0, 0, "pool.ntp.org");
    
  // Set timezone to Eastern Standard Time (EST) with Daylight Saving Time (EDT)
  // The third parameter '1' overwrites any existing TZ environment variable.
  setenv("TZ", "EST5EDT", 1);
  tzset();
  // Wait for the time to be synchronized
  time_t now = time(nullptr);
  while (now < 10000) {
    delay(500);
    now = time(nullptr);
  }
}

void setup() {
  delay(2000); // Wait for the display to stabilize
  
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }

  syncTime();
  
  display.init();
  display.setRotation(0); // Landscape

  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();

  // Normal initialization: clear the display to a known blank state.
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
  } while (display.nextPage());
  
}

void loop() {
  static int loopCount = 0;
  for (int i = 0; i < 5; i++)
  {
    // Set partial window for this quadrant
    display.setPartialWindow(
      displayAreas[i].x,
      displayAreas[i].y,
      displayAreas[i].w,
      displayAreas[i].h
    );

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
        update = app.shouldUpdateNhl();
        break;
      case 3:
        update = app.shouldUpdateBlynk();
        break;
      case 4:
        if(loopCount % 60 == 0) { // Update every 60 loops
          update = true;
        }
    }

    // Only refresh if something was drawn
    if (update) {
      display.firstPage();
      do {
        display.fillScreen(GxEPD_WHITE);
        display.setCursor(displayAreas[i].x + 5, displayAreas[i].y + 20);
        switch (i) {
          case 0:
            app.renderGoogleInfo(display);
            break;
          case 1:
            app.renderMarketInfo(display);
            break;
          case 2:
            app.renderNhlInfo(display);
            break;
          case 3:
            app.renderBlynkInfo(display);
            break;
          case 4:
            drawMemoryStats(display);
            break;
        }
      } while (display.nextPage());
    }
  }
  delay(1000); // Optional: slow down loop
  loopCount++;
}

