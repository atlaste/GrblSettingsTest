#pragma once

#include "Assert.h"
#include "Configuration/GenericFactory.h"
#include "Configuration/HandlerBase.h"

#include <vector>

#include <iostream>

class I2SO : public Configuration::Configurable
{
    Pin bck_;
    Pin data_;
    Pin ws_;

public:
    I2SO() = default;

    void validate() const override {
        if (!bck_.undefined() ||
            !data_.undefined() ||
            !ws_.undefined())
        {
            Assert(bck_.undefined(), "I2SO BCK pin should be configured once.");
            Assert(data_.undefined(), "I2SO Data pin should be configured once.");
            Assert(ws_.undefined(), "I2SO WS pin should be configured once.");
        }
    }

    void handle(Configuration::HandlerBase& handler) override {
        handler.handle("bck", bck_);
        handler.handle("data", data_);
        handler.handle("ws", ws_);
    }

    // const char* name() const override { return "i2so"; }

    ~I2SO() {}
};

class Motor : public Configuration::Configurable {
};

using MotorFactory = Configuration::GenericFactory<Motor>;

class Stepstick : public Motor
{
    friend class Configuration::HandlerBase;

    Pin step_;
    Pin direction_;
    Pin enable_;

public:
    Stepstick() = default;

    void validate() const override {
        Assert(!step_.undefined(), "Stepper step pin should be configured.");
        Assert(!direction_.undefined(), "Direction pin should be configured.");
    }

    void handle(Configuration::HandlerBase& handler) override {
        handler.handle("step", step_);
        handler.handle("direction", direction_);
        handler.handle("enable", enable_);
    }

    // const char* name() const override { return "stepstick"; }
    // TODO FIXME: should be in a cpp file, and: return stepstickRegistration.name();
};

namespace {
    MotorFactory::InstanceBuilder<Stepstick> stepstickRegistration("stepstick");
}

class Axis : public Configuration::Configurable {
    Pin endstop_;
    Motor* motor_ = nullptr;

public:
    Axis() = default;

    void validate() const override {
        Assert(motor_ != nullptr, "Motor should be defined when an axis is defined.");
    }

    void handle(Configuration::HandlerBase& handler) override {
        handler.handle("endstop", endstop_);
        MotorFactory::handle(handler, motor_);
    }

    // const char* name() const override { return "axis"; }

    ~Axis() {
        delete motor_;
    }
};

class Axes : public Configuration::Configurable {
    static const int MAX_NUMBER_AXIS = 6;
    static const int MAX_NUMBER_GANGED = 2;

    Axis* axis_[MAX_NUMBER_AXIS][MAX_NUMBER_GANGED + 1];

public:
    Axes() : axis_()
    {
        for (int i = 0; i < MAX_NUMBER_AXIS; ++i)
        {
            for (int j = 0; j <= MAX_NUMBER_GANGED; ++j)
            {
                axis_[i][j] = nullptr;
            }
        }
    }

    void validate() const override { }

    void handle(Configuration::HandlerBase& handler) override {
        const char* allAxis = "xyzabc";

        char tmp[3];
        tmp[2] = '\0';

        for (size_t a = 0; a < MAX_NUMBER_AXIS; ++a)
        {
            tmp[0] = allAxis[a];
            tmp[1] = '\0';

            if (handler.handlerType() == Configuration::HandlerType::Runtime || 
                handler.handlerType() == Configuration::HandlerType::Parser)
            {
                // 'x' is a shorthand for 'x1', so we don't generate it.
                handler.handle(tmp, axis_[a][1]);
            }

            for (size_t g = 1; g <= MAX_NUMBER_GANGED; ++g)
            {
                tmp[1] = char('0' + g);
                handler.handle(tmp, axis_[a][g]);
            }
        }
    }

    // const char* name() const override { return "machine"; }

    ~Axes() {
        for (int i = 0; i < MAX_NUMBER_AXIS; ++i)
        {
            for (int j = 0; j <= MAX_NUMBER_GANGED; ++j)
            {
                delete axis_[i][j];
            }
        }
    }
};

class Machine : public Configuration::Configurable {
private:
    I2SO* i2so_ = nullptr;
    Axes* axes_ = nullptr;

public:
    Machine() = default;

    void validate() const override { }

    void handle(Configuration::HandlerBase& handler) override {
        handler.handle("i2so", i2so_);
        handler.handle("axes", axes_);
    }

    // const char* name() const override { return "machine"; }

    ~Machine() {
        delete i2so_;
        delete axes_;
    }
};