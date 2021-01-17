#pragma once

#include "StringRange.h"
#include <string>

class Pin {
    std::string str_;  // stub

public:
    Pin() {}
    Pin(StringRange str) : str_(str.begin(), str.end()) {} // stub

    inline bool undefined() const { return str_.empty(); }

    const char* str() const { return str_.c_str(); }

    ~Pin() {}
};