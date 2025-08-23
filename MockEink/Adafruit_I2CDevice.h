#ifndef _ADAFRUIT_I2C_DEVICE_H_
#define _ADAFRUIT_I2C_DEVICE_H_

#include <cstdint>
#include "Arduino.h"

// Define a placeholder class to satisfy the compiler.
class Adafruit_I2CDevice {
public:
    Adafruit_I2CDevice() {}
    ~Adafruit_I2CDevice() {}

    bool begin() { return true; }
};

#endif