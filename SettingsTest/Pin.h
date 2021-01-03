#pragma once

#include <string>

class Pin {
    std::string str_;

public:
    Pin() {}
    Pin(const std::string& str) : str_(str) {}

    inline bool undefined() const { return str_.empty(); }

    const char* str() const { return str_.c_str(); }
};