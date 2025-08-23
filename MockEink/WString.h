#ifndef WSTRING_H
#define WSTRING_H

#include "Arduino.h"
#include <string>

// A mock Arduino String class
class String {
public:
    String() {}
    String(const char* c_str) : s(c_str) {}
    String(const std::string& str) : s(str) {}

    // Required for GFX library functions like print()
    operator const char*() const { return s.c_str(); }

    std::string s;
};

#endif