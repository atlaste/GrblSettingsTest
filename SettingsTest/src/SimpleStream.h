#pragma once

#include <cstring>

#include "StringRange.h"
#include "Pin.h"

class SimpleStream
{
    static char* intToBuf(int value, char* dst);

public:
    SimpleStream() = default;

    SimpleStream(const SimpleStream& o) = delete;
    SimpleStream(SimpleStream&& o) = delete;

    SimpleStream& operator=(const SimpleStream& o) = delete;
    SimpleStream& operator=(SimpleStream&& o) = delete;

    virtual void add(char c) = 0;
    virtual void flush() {}

    void add(const char* s);
    void add(int value);
    void add(double value, int numberDigits, int precision);
    void add(StringRange range);
    void add(const Pin& pin);

    virtual ~SimpleStream() {
    }
};

inline SimpleStream& operator<<(SimpleStream& lhs, char c) {
    lhs.add(c);
    return lhs;
}

inline SimpleStream& operator<<(SimpleStream& lhs, const char* v) {
    lhs.add(v);
    return lhs;
}

inline SimpleStream& operator<<(SimpleStream& lhs, int v) {
    lhs.add(v);
    return lhs;
}

inline SimpleStream& operator<<(SimpleStream& lhs, double v) {
    lhs.add(v, 4, 3);
    return lhs;
}

inline SimpleStream& operator<<(SimpleStream& lhs, StringRange v) {
    lhs.add(v);
    return lhs;
}

inline SimpleStream& operator<<(SimpleStream& lhs, const Pin& v) {
    lhs.add(v);
    return lhs;
}