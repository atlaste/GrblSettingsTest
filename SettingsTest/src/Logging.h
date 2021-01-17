#pragma once

#include "SimpleStream.h"

#ifndef ESP32

#include <iostream>

class DebugStream : public SimpleStream
{
public:
    DebugStream(const char* name) {
        std::cout << '[' << name << ": ";
    }
    void add(char c) override
    {
        std::cout << c;
    }

    ~DebugStream() { std::cout << ']' << std::endl; }
};

#else

class DebugStream : public SimpleStream
{
public:
    DebugStream(const char* name) {
        Serial.print("[");
        Serial.print(name);
        Serial.print(": ");
    }

    void add(char c) override { Serial.print(c); }

    ~DebugStream() { Serial.println("]"); }
};

#endif

#include "StringStream.h"

#define debug(x) { DebugStream ss("DBG "); ss << x; }
#define info(x)  { DebugStream ss("INFO"); ss << x; }
#define warn(x)  { DebugStream ss("WARN"); ss << x; }
#define error(x) { DebugStream ss("ERR "); ss << x; }

