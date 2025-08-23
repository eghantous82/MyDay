#ifndef MOCK_EPAPER_DISPLAY_H
#define MOCK_EPAPER_DISPLAY_H

#include "Adafruit_GFX.h"
#include <stdint.h>
#include <vector>
#include "Print.h" // Include the Print header

class MockEpaperDisplay : public Adafruit_GFX { // Inherit from Print
public:
    MockEpaperDisplay(int16_t w, int16_t h);
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void fillScreen(uint16_t color);
    void saveToFile(const std::string& filename);

    // Override the virtual write method from the Print class
    size_t write(uint8_t c) override;

    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;


private:
    std::vector<uint8_t> buffer;
    int16_t _width;
    int16_t _height;
};
#endif