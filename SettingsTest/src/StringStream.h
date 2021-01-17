#pragma once

#include "SimpleStream.h"
#include "StringRange.h"

#include <vector>

class StringStream : public SimpleStream
{
    std::vector<char> data_;

public: 
    void add(char c) override {
        data_.push_back(c);
    }

    StringRange str() const {
        return StringRange(data_.data(), data_.data() + data_.size());
    }
};