#ifndef _PRINT_H_
#define _PRINT_H_

#include "Arduino.h"

// Define a placeholder class to satisfy the compiler.
class Print {
public:
    virtual size_t write(uint8_t) { return 1; }
    virtual size_t write(const uint8_t *buffer, size_t size) { return size; }
};

#endif