#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <WiFi.h>
#include "Application.h"
#include "SecretTokens.h"
#include "Secrets.h"
#include "Esp32DataRetriever.h"

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=*/3, /*DC=*/5, /*RST=*/2, /*BUSY=*/4));
void setup() {
  delay(2000); // Wait for the display to stabilize
  
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());

  // Waiting the connection to a router
 
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }

  Secrets secrets(SECRET_TOKENS);
  Esp32DataRetriever retriever;
  Application app(retriever, secrets);
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
  
}

void loop() {
  // Nothing here
}