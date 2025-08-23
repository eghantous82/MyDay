#ifndef ARDUINO_H
#define ARDUINO_H

#include <cstdint>
#include <string> // You might need this for std::string

// Define a placeholder class for __FlashStringHelper
class __FlashStringHelper;

// Define standard types used in Arduino sketches.
typedef uint8_t byte;
typedef uint16_t word;

// Define a few constants, if needed by the GFX library.
#define PROGMEM
#define PSTR(x) (x)
#define F(x) static_cast<const __FlashStringHelper*>(x)

// A few simple function definitions to satisfy the compiler
template <typename T>
T inline min(T a, T b) { return a < b ? a : b; }

template <typename T>
T inline max(T a, T b) { return a > b ? a : b; }

template <typename T>
T inline abs(T a) { return a > 0 ? a : -a; }

#endif