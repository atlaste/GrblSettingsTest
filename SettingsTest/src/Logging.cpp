#include "Logging.h"

#include <cstdio>
#include <cstdarg>
#include <cassert>

#ifdef ESP32

// #include <Arduino.h>

void print(const char* msg ) {
    //Serial.println(msg);
}

#else

void print(const char* msg) {
    printf("%s\n", msg);
}

#endif

void debug(const char* fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, 256, fmt, args);
    va_end(args);

    print(buffer);
}

void info(const char* fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, 256, fmt, args);
    va_end(args);

    print(buffer);
}

void warn(const char* fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, 256, fmt, args);
    va_end(args);

    print(buffer);
}

void error(const char* fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, 256, fmt, args);
    va_end(args);

    print(buffer);
}
