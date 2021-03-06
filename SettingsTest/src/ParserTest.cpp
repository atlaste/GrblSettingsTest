#ifndef ESP32

#include "Configuration/Parser.h"
#include "Configuration/ParserHandler.h"
#include "Configuration/ParseException.h"
#include "Configuration/Configurable.h"
#include "Configuration/RuntimeSetting.h"
#include "Configuration/LegacySettingRegistry.h"
#include "Configuration/Validator.h"
#include "Assert.h"

#include "Machine.h"

#include <string>
#include <fstream>
#include <iostream>
#include <streambuf>

int main() {
    std::ifstream t("..\\SettingsTest\\Test.yaml");
    std::string   ts((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    auto input = StringRange(ts.data(), ts.data() + ts.size());

    try {
        Configuration::Parser parser(input.begin(), input.end());
        Configuration::ParserHandler handler(parser);

        Machine machine;
        for (; !parser.isEndSection(); parser.moveNext())
        {
            std::cout << "Parsing key " << parser.key().str() << std::endl;
            machine.handle(handler);
        }

        std::cout << "Done parsing machine config." << std::endl;

        try {
            Configuration::Validator validator;
            machine.handle(validator);
        }
        catch (std::exception& ex)
        {
            std::cout << "Validation error: " << ex.what() << std::endl;
        }

        std::cout << "Done validating machine config." << std::endl;

        {
            StringStream ss;
            
            Configuration::Generator generator(ss);
            machine.handle(generator);

            std::cout << "Complete config: " << std::endl;
            std::cout << ss.str().str() << std::endl;

            std::cout << "Done generating machine config." << std::endl;
        }

        while (true)
        {
            std::string s;
            std::getline(std::cin, s);
            if (!s.size())
            {
                break;
            }

            if (s[0] == '$')
            {
                // Try legacy handling first, then runtime setting handling:
                if (!Configuration::LegacySettingRegistry::tryHandleLegacy(s.c_str()))
                {
                    Configuration::RuntimeSetting setting(s.c_str());
                    machine.handle(setting);
                }

                // Done. Regenerate config:
                {
                    StringStream ss;

                    Configuration::Generator generator(ss);
                    machine.handle(generator);

                    std::cout << "Complete config: " << std::endl;
                    std::cout << ss.str().str() << std::endl;
                }
            }
        }

    }
    catch (const Configuration::ParseException& ex) {
        std::cout << "Parse error: " << ex.What() << " @ " << ex.LineNumber() << ":" << ex.ColumnNumber() << std::endl;
    }
    catch (const std::exception& ex) {
        std::cout << "Parse error: " << ex.what() << std::endl;
    }
    catch (...) { std::cout << "Uncaught exception" << std::endl; }

    // std::string s;
    // std::getline(std::cin, s);
    return 0;
}

#endif