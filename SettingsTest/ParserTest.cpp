#include "Configuration/Parser.h"
#include "Configuration/ParseException.h"

#include "Machine.h"

#include <string>
#include <fstream>
#include <iostream>
#include <streambuf>

/*
void ParseBus(Parser& parser) {
    std::cout << "Parsing bus." << std::endl;

    // TODO
}

void ParseRoot(Parser& parser) {
    std::cout << "Parsing root." << std::endl;

    for (; !parser.IsEndSection(); parser.moveNext()) {
        if (parser.is("axis")) {
            parser.enter();
            ParseAxis(parser);
            parser.leave();
        }
        else if (parser.is("bus")) {
            parser.enter();
            ParseBus(parser);
            parser.leave();
        }
    }
}

*/
int main() {
    std::ifstream t("..\\SettingsTest\\Test.yaml");
    std::string   str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    const auto begin = str.c_str();
    const auto end = begin + str.size();

    try {
        Configuration::Parser parser(begin, end);
        
        Machine machine(parser);

        std::cout << "Done parsing machine config." << std::endl;

        Configuration::Generator generator;
        machine.generate(generator);
        std::cout << "Complete config: " << std::endl;
        std::cout << generator.str() << std::endl;
    }
    catch (const Configuration::ParseException& ex) {
        std::cout << "Parse error: " << ex.What() << " @ " << ex.LineNumber() << ":" << ex.ColumnNumber() << std::endl;
    }
    catch (const std::exception& ex) {
        std::cout << "Parse error: " << ex.what() << std::endl;
    }
    catch (...) { std::cout << "Uncaught exception" << std::endl; }

    std::string s;
    std::getline(std::cin, s);
    return 0;

    return 0;
}
