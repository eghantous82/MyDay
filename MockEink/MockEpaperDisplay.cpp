#include "MockEpaperDisplay.h"
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

MockEpaperDisplay::MockEpaperDisplay(int16_t w, int16_t h) : Adafruit_GFX(w, h), _width(w), _height(h) {
    buffer.resize(w * h, 0);
}

void MockEpaperDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;
    buffer[x + y * _width] = (color > 0) ? 255 : 0;
}

void MockEpaperDisplay::fillScreen(uint16_t color) {
    uint8_t val = (color > 0) ? 255 : 0;
    std::fill(buffer.begin(), buffer.end(), val);
}

// Assuming _width, _height, and buffer are class members of MockEpaperDisplay
void MockEpaperDisplay::saveToFile(const std::string& filename) {
    // We use a try-catch block for robust error handling
    try {
        // Create an 8-bit, single-channel (grayscale) OpenCV Mat object.
        // The Mat constructor takes height first, then width.
        cv::Mat img(_height, _width, CV_8UC1);

        // Copy the pixel data from your buffer to the OpenCV Mat object.
        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x) {
                // Invert colors to match the e-paper display's logic (0 = black, 255 = white)
                // The .at<unsigned char>(y, x) method provides safe, direct pixel access.
                img.at<unsigned char>(y, x) = 255 - buffer[x + y * _width];
            }
        }

        // Save the image to the specified file path.
        // The return value indicates if the save operation was successful.
        bool success = cv::imwrite(filename, img);

        if (!success) {
            std::cerr << "Error: Could not save the image to " << filename << std::endl;
        }

    } catch (const cv::Exception& e) {
        // Catch any OpenCV-specific exceptions and print the error message.
        std::cerr << "OpenCV Exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        // Catch any other standard exceptions.
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    }
}

size_t MockEpaperDisplay::write(uint8_t c) {
    // This calls the write() function from Adafruit_GFX, which handles
    // drawing the character to the display buffer.
    return Adafruit_GFX::write(c);
}

void MockEpaperDisplay::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    // Check for out-of-bounds coordinates to prevent crashes
    if (y < 0 || y >= _height) return;
    // Iterate through the width and draw a pixel at each point
    for (int16_t i = 0; i < w; i++) {
        drawPixel(x + i, y, color);
    }
}

// Draw a vertical line
void MockEpaperDisplay::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    // Check for out-of-bounds coordinates
    if (x < 0 || x >= _width) return;
    // Iterate through the height and draw a pixel at each point
    for (int16_t i = 0; i < h; i++) {
        drawPixel(x, y + i, color);
    }
}

void MockEpaperDisplay::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h - 1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w - 1, y, h, color);
}
