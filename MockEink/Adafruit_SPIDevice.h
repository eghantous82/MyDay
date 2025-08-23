#ifndef _ADAFRUIT_SPI_DEVICE_H_
#define _ADAFRUIT_SPI_DEVICE_H_

#include <cstdint>
#include "Arduino.h"

// Define a placeholder class to satisfy the compiler.
class Adafruit_SPIDevice {
public:
    Adafruit_SPIDevice() {}
    ~Adafruit_SPIDevice() {}

    bool begin() { return true; }
};

#endif