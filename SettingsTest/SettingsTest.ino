#include "src/Configuration/Parser.h"
#include "src/Configuration/ParserHandler.h"
#include "src/Configuration/ParseException.h"
#include "src/Configuration/Configurable.h"
#include "src/Configuration/Validator.h"
#include "src/Assert.h"

#include "src/Machine.h"


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

        StringStream ss;

        Configuration::Generator generator(ss);
        machine.handle(generator);

        auto str = ss.str();
        debug("%s", str.str().c_str());
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