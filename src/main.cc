#include "interp.hh"
#include <iostream>
#include <string>
#include <vector>

namespace {
    using namespace esquema;
    using namespace std::literals::string_literals;
    std::string buffer(1024, ' ');
    int indent_size{2};
    struct ParseTreePrinter {
    public:
        static std::string_view pre(int indent) noexcept {
            if (indent_size * indent < 1024) {
                return std::string_view(buffer.c_str(), indent_size * indent);
            }
            else {
                return buffer;
            }
        }

    public:
        void operator()(Cell const & cell, int depth) {
            if (cell.is_list()) {
                (*this)(std::get<List>(cell), depth + 1);
            }
            else if (cell.is_symbol()) {
                (*this)(std::get<Symbol>(cell), depth + 1);
            }
            else if (cell.is_number()) {
                (*this)(std::get<Number>(cell), depth + 1);
            }
            else if (cell.is_proc()) {
                (*this)(std::get<Proc>(cell), depth + 1);
            }
        }

        void operator()(List const & list, int depth) {
            std::cout << pre(depth) << "List \n";
            for (auto const & x : list) {
                (*this)(x, depth + 1);
            }
            std::cout << "\n";
        }

        void operator()(Symbol const & symbol, int depth) {
            std::cout << pre(depth) << symbol.value() << "\n";
        }

        void operator()(Number const & number, int depth) {
            std::cout << pre(depth) << number.value() << "\n";
        }

        void operator()(Proc, int depth) {
            std::cout << pre(depth) << "Proc\n";
        }
    };
}

int main(int argc, char ** argv) {
    auto programs = std::vector<std::string>{
        "(define x (+ +2.1234 -.2))"s, 
        "x"s,
        "(+ 2 2)"s,
        "(+ (+ 1 2 3) (+ 1 2) 5)"s,
        "(* 1 2 3 4 5 6)"s,
        "2"s,
        "()"s,
        ""s
    };

    esquema::Interpreter interpreter{};
    for (auto const & program : programs) {
        std::cout << interpreter.eval(program) << "\n";
    }

    return EXIT_SUCCESS;
}
