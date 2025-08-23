#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// Use the constructor directly for 7.5" B/W display (GDEW075T8)
GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=*/7, /*DC=*/6, /*RST=*/5, /*BUSY=*/4));

void setup() {
  delay(2000); // Wait for the display to stabilize
  
  Serial.begin(115200);
  Serial.println("Displayed 'Hello World' on E-Ink. 1");
  display.init();
  display.setRotation(1); // Landscape

  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.firstPage();
  Serial.println("Displayed 'Hello World' on E-Ink. 2");
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(50, 100);
    display.print("Hello World");
    Serial.println("Displayed 'Hello World' on E-Ink. 3");
  } while (display.nextPage());

  Serial.println("Displayed 'Hello World' on E-Ink.");
  
}

void loop() {
  // Nothing here
}