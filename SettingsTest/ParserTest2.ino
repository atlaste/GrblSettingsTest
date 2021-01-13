#include "Configuration/Parser.h"
#include "Configuration/ParserHandler.h"
#include "Configuration/ParseException.h"
#include "Configuration/Configurable.h"
#include "Configuration/Validator.h"
#include "Assert.h"

#include "Machine.h"


void setup() {
    std::string   str("name: board"); // doesn't really matter.

    const auto begin = str.c_str();
    const auto end = begin + str.size();

    try {
        Configuration::Parser parser(begin, end);
        Configuration::ParserHandler handler(parser);

        Machine machine;
        for (; !parser.isEndSection(); parser.moveNext())
        {
            machine.handle(handler);
        }

        try {
            Configuration::Validator validator;
            machine.handle(validator);
        }
        catch (std::exception& ex)
        {
            error("Validation error: %s", ex.what());
        }

        Configuration::Generator generator;
        machine.handle(generator);

        auto str = generator.str();
        debug("%s", str.c_str());

    }
    catch (const Configuration::ParseException& ex) {
        // std::cout << "Parse error: " << ex.What() << " @ " << ex.LineNumber() << ":" << ex.ColumnNumber() << std::endl;
    }
    catch (const std::exception& ex) {
        // std::cout << "Parse error: " << ex.what() << std::endl;
    }
    catch (...) { 
        // std::cout << "Uncaught exception" << std::endl; 
    }
}

void loop() {}