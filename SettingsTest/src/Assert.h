#pragma once

class AssertionFailed
{
    const char* message_;

public:
    AssertionFailed(const char* msg) : message_(msg) {}

    const char* what() const { return message_; }
};

#undef Assert

#define Stringify(x) #x
#define Stringify2(x) Stringify(x)
#define Assert(condition, ...)                                                                                                             \
    {                                                                                                                                      \
        if (!(condition)) {                                                                                                                \
            const char* ch = #condition " (@line " Stringify2(__LINE__) ")";                                                                \
            throw AssertionFailed(ch);                                                                              \
        }                                                                                                                                  \
    }
