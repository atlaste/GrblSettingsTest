#pragma once

#include <vector>

#include "../Pin.h"
#include "HandlerBase.h"
#include "../StringRange.h"

namespace Configuration
{
    class Configurable;

    class Generator : public HandlerBase
    {
        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        std::vector<char> config_;
        int indent_;
        bool lastIsNewline_ = false;

        inline void addStr(const char* text) {
            lastIsNewline_ = false;
            for (auto it = text; *it; ++it)
            {
                config_.push_back(*it);
            }
        }

        inline void addStr(StringRange text)
        {
            lastIsNewline_ = false;
            for (auto it : text) {
                config_.push_back(it);
            }
        }

        inline void indent() {
            for (int i = 0; i < indent_ * 2; ++i)
            {
                config_.push_back(' ');
            }
        }

        void enter(const char* name);
        void add(const char* key, StringRange value);
        void add(const char* key, const char* value);
        void add(const char* key, bool value);
        void add(const char* key, int value);
        void add(const char* key, double value);
        void add(const char* key, const Pin& value);
        void add(Configuration::Configurable* configurable);
        void leave();
        
    protected:
        void handleDetail(const char* name, Configurable* value) override;
        bool matchesUninitialized(const char* name) override { return false; }
        HandlerType handlerType() override { return HandlerType::Generator; }

    public:
        Generator() : indent_(0) {}

        void handle(const char* name, int& value) override {
            add(name, value);
        }

        void handle(const char* name, double& value) override {
            add(name, value);
        }

        void handle(const char* name, StringRange value) override {
            add(name, value);
        }

        void handle(const char* name, Pin& value) override {
            add(name, value);
        }

        inline StringRange str() const {
            return StringRange(config_.data(), config_.data() + config_.size());
        }
    };
}