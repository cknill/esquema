#include "interp.hh"
#include "linenoise.hpp"
#include <exception>
#include <iostream>
#include <string>

namespace {
    using namespace std::literals::string_view_literals;
    using namespace std::literals::string_literals;
    using namespace esquema;
}

int main(int argc, char ** argv) {
    Interpreter interpreter{};
    std::string line;
    std::cout << "Bienvenidos to the Esquema REPL \n"sv
              << "type an expression to evaluate or type \n"sv 
              << "':q' or use 'Ctrl+c' to quit\n"sv
              << "----------------------------------------\n\n"sv;
    while (true) {
        auto quit = linenoise::Readline("esquema> ", line);
        if (quit) {
            break;
        }

        if (line.empty()) {
            continue;
        }
        // TODO - This is like Haskell's REPL
        // Scheme might do it some other way.
        else if (line == ":q"sv) {
            break;
        }

        try {
            std::cout << interpreter.eval(line) << '\n';
        }

        catch (std::exception const & ex) {
            std::cerr << ex.what() << '\n';
        }
        catch (...) {
            std::cerr << "Unexpected error, exiting\n"sv;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
