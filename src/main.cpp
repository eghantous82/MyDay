#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <WiFi.h>
#include <time.h>
#include "Application.h"
#include "SecretTokens.h"
#include "Secrets.h"
#include "Esp32DataRetriever.h"


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

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=*/3, /*DC=*/5, /*RST=*/2, /*BUSY=*/4));

RTC_DATA_ATTR time_t _lastRunTime = 0;

void setup() {
  delay(2000); // Wait for the display to stabilize
  
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());

  // Waiting the connection to a router
 
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }

  syncTime();
  
  // Get the current time for the calculation
  time_t currentTime = time(NULL);

  Secrets secrets(SECRET_TOKENS);
  Esp32DataRetriever retriever;
  Application app(retriever, secrets, _lastRunTime);
  display.init();
  display.setRotation(0); // Landscape

  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    app.renderScreen(display);
  } while (display.nextPage());
  
  display.hibernate(); // Save power

  _lastRunTime = currentTime;

  // Sleep for 30 minutes (in microseconds)
  esp_sleep_enable_timer_wakeup(30ULL * 60ULL * 1000000ULL);
  esp_deep_sleep_start();
}

void loop() {
  // Nothing here
}