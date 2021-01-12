#pragma once

#include <string>
#include <vector>

#include "../Pin.h"
#include "HandlerBase.h"

namespace Configuration
{
    class Configurable;

    class Generator : public HandlerBase
    {
        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        std::vector<char> config_;
        int indent_;

        inline void addStr(const char* text) {
            for (auto it = text; *it; ++it)
            {
                config_.push_back(*it);
            }
        }

        inline void indent() {
            for (int i = 0; i < indent_ * 2; ++i)
            {
                config_.push_back(' ');
            }
        }

        void enter(const char* name);
        void add(const char* key, const std::string& value);
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
        bool isBuilder() override { return false; }

    public:
        Generator() = default;

        void handle(const char* name, int& value) override {
            add(name, value);
        }

        void handle(const char* name, double& value) override {
            add(name, value);
        }

        void handle(const char* name, std::string& value) override {
            add(name, value);
        }

        void handle(const char* name, Pin& value) override {
            add(name, value);
        }

        inline std::string str() const {
            return std::string(config_.begin(), config_.end());
        }
    };
}