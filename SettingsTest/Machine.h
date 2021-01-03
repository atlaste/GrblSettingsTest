#pragma once

#include "Assert.h"
#include "Configuration/Configurable.h"
#include "Configuration/GenericFactory.h"

#include <vector>

#include <iostream>

class I2SO : public Configuration::Configurable
{
    Pin bck_;
    Pin data_;
    Pin ws_;

public:
    I2SO(Configuration::Parser& parser) {
        for (; !parser.isEndSection(); parser.moveNext()) {
            if (parser.is("bck")) {
                Assert(bck_.undefined(), "I2SO BCK pin should be configured once.");
                bck_ = parser.pinValue();
            }
            else if (parser.is("data")) {
                Assert(data_.undefined(), "I2SO Data pin should be configured once.");
                data_ = parser.pinValue();
            }
            else if (parser.is("ws")) {
                Assert(ws_.undefined(), "I2SO WS pin should be configured once.");
                ws_ = parser.pinValue();
            }
        }
    }

    void generate(Configuration::Generator& generator) const override {
        generator.enter("i2so");
        generator.add("bck", bck_);
        generator.add("data", data_);
        generator.add("ws", ws_);
        generator.leave();
    }

    ~I2SO() {}
};

class Motor : public Configuration::Configurable {
};

using MotorFactory = Configuration::GenericFactory<Motor>;

class Stepstick : public Motor
{
    Pin step_;
    Pin direction_;
    Pin enable_;

public:
    Stepstick(Configuration::Parser& parser) {
        for (; !parser.isEndSection(); parser.moveNext()) {
            if (parser.is("step")) {
                Assert(step_.undefined(), "Stepper step pin should be configured once.");
                step_ = parser.pinValue();
                std::cout << "Step pin: " << step_.str() << std::endl;
            }
            else if (parser.is("direction")) {
                Assert(direction_.undefined(), "Stepper direction pin should be configured once.");
                direction_ = parser.pinValue();
                std::cout << "Direction pin: " << direction_.str() << std::endl;
            }
            else if (parser.is("enable")) {
                Assert(enable_.undefined(), "Stepper enable pin should be configured once.");
                enable_ = parser.pinValue();
                std::cout << "Enable pin: " << enable_.str() << std::endl;
            }
        }
    }

    void generate(Configuration::Generator& generator) const override {
        generator.enter("stepstick");
        generator.add("step", step_);
        generator.add("direction", direction_);
        generator.add("enable", enable_);
        generator.leave();
    }
};

namespace {
    MotorFactory::InstanceBuilder<Stepstick> stepstickRegistration("stepstick");
}

class Axis : public Configuration::Configurable {
    Pin endstop_;
    Motor* motor_ = nullptr;

public:
    Axis(Configuration::Parser& parser) {
        for (; !parser.isEndSection(); parser.moveNext()) {
            if (parser.is("endstop")) {
                endstop_ = parser.pinValue();
                std::cout << "Endstop: " << endstop_.str() << std::endl;
            }
            else {
                auto key = parser.key();
                auto motorBuilder = MotorFactory::find(key);
                if (motorBuilder)
                {
                    Assert(motor_ == nullptr, "Only 1 motor is allowed per axis/ganged combination");

                    std::cout << "Building motor: " << key << std::endl;
                    parser.enter();
                    motor_ = motorBuilder->create(parser);
                    parser.leave();
                }
            }
        }
    }

    void generate(Configuration::Generator& generator) const override {
        generator.add("endstop", endstop_);
        generator.add(motor_);
    }

    ~Axis() {
        delete motor_;
    }
};

class Machine : public Configuration::Configurable {
private:
    static const int MAX_NUMBER_AXIS = 6;
    static const int MAX_NUMBER_GANGED = 2;

    I2SO* i2so_ = nullptr;
    Axis* axis_[MAX_NUMBER_AXIS + 1][MAX_NUMBER_GANGED + 1];

public:
    Machine(Configuration::Parser& parser) : axis_() {
        for (int i = 0; i <= MAX_NUMBER_AXIS; ++i)
        {
            for (int j = 0; j <= MAX_NUMBER_GANGED; ++j)
            {
                axis_[i][j] = nullptr;
            }
        }

        const char* allAxis = "xyzabc";

        for (; !parser.isEndSection(); parser.moveNext()) {
            if (parser.is("i2so")) {
                Assert(i2so_ == nullptr, "I2SO is defined multiple times.");

                parser.enter();
                i2so_ = new I2SO(parser);
                parser.leave();
            }
            else if (parser.is("axis")) {
                parser.enter();

                for (; !parser.isEndSection(); parser.moveNext()) {
                    auto str = parser.key();
                    if (str.size() == 1) {
                        auto idx = strchr(allAxis, str[0]);
                        if (idx != nullptr) {
                            parser.enter();

                            int axisIndex = idx - allAxis;
                            int ganged = 0;

                            Assert(axisIndex >= 0 && axisIndex <= MAX_NUMBER_AXIS, "Axis is out of the range for number of axis");
                            Assert(axis_[axisIndex][ganged] == nullptr, "Expected axis/ganged combination to be defined once");

                            std::cout << "Parsing axis=" << allAxis[axisIndex] << ", ganged=" << ganged << std::endl;

                            axis_[axisIndex][ganged] = new Axis(parser);
                            parser.leave();
                        }
                    }
                    else if (str.size() == 2) {
                        auto idx = strchr(allAxis, str[0]);
                        if (idx != nullptr && str[1] >= '1' && str[1] <= '9') {
                            int ganged = str[1] - '1';

                            parser.enter();

                            int axisIndex = idx - allAxis;

                            Assert(axisIndex >= 0 && axisIndex <= MAX_NUMBER_AXIS, "Axis is out of the range for number of axis");
                            Assert(ganged >= 0 && ganged <= MAX_NUMBER_GANGED, "Ganged is out of the range for number of ganged per axis");
                            Assert(axis_[axisIndex][ganged] == nullptr, "Expected axis/ganged combination to be defined once");

                            std::cout << "Parsing axis=" << allAxis[axisIndex] << ", ganged=" << ganged << std::endl;

                            axis_[axisIndex][ganged] = new Axis(parser);
                            parser.leave();
                        }
                    }
                }

                parser.leave();
            }
        }
    }

    void generate(Configuration::Generator& generator) const override {
        generator.add(i2so_);

        const char* allAxis = "xyzabc";
        generator.enter("axis");
        for (int i = 0; i <= MAX_NUMBER_AXIS; ++i)
        {
            for (int j = 0; j <= MAX_NUMBER_GANGED; ++j)
            {
                auto current = axis_[i][j];
                if (current) {
                    char buf[3];
                    buf[0] = allAxis[i];
                    buf[1] = char(j + '0');
                    buf[2] = '\0';

                    generator.enter(buf);
                    current->generate(generator);
                    generator.leave();
                }
            }
        }
        generator.leave();
    }

    ~Machine() {
        delete i2so_;
        for (int i = 0; i <= MAX_NUMBER_AXIS; ++i)
        {
            for (int j = 0; j <= MAX_NUMBER_GANGED; ++j)
            {
                delete axis_[i][j];
            }
        }
    }
};