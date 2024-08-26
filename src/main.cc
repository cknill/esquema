#include "interp.hh"
#include "linenoise.hpp"
#include <iostream>
#include <string>

namespace {
    using namespace std::literals::string_literals;
}

int main(int argc, char ** argv) {
    esquema::Interpreter interpreter{};
    std::string line;
    while (true) {
        auto quit = linenoise::Readline("esquema> ", line);
        if (quit) {
            break;
        }

        std::cout << interpreter.eval(line) << '\n';
    }

    return EXIT_SUCCESS;
}
