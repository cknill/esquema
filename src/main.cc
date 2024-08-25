#include <iostream>
#include <string>
#include <vector>

namespace {
    using namespace std::literals::string_literals;
}

int main(int argc, char ** argv) {
    auto programs = std::vector<std::string>{
        "(define x (+ +2.1234 -.2))"s, 
        "x"s,
        "(+ 2 2)"s,
        "2"s,
        "()"s,
        ""s
    };

    return EXIT_SUCCESS;
}
