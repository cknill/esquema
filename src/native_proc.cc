#include "native_proc.hh"
#include "interp.hh"

#include <sstream>
#include <stdexcept>

namespace esquema {
    Cell add(List const & args, Environment * env) {
        auto value = 0.0D;
        for (auto const & arg : args) {
            if (!arg.is_number()) {
                std::ostringstream msg{};
                msg << "Type error: Expected number got " << arg;
                throw std::runtime_error{msg.str()};
            }

            value += std::get<Number>(arg).value();
        }

        return Number{value};
    }

    Cell sub(List const & args, Environment * env) {
        auto value = 0.0D;
        for (auto const & arg : args) {
            if (!arg.is_number()) {
                std::ostringstream msg{};
                msg << "Type error: Expected number got " << arg;

                throw std::runtime_error{msg.str()};
            }

            value -= std::get<Number>(arg).value();
        }

        return Number{value};
    }

    Cell mul(List const & args, Environment * env) {
        auto value = 1.0D;
        for (auto const & arg : args) {
            if (!arg.is_number()) {
                std::ostringstream msg{};
                msg << "Type error: Expected number got " << arg;

                throw std::runtime_error{msg.str()};
            }

            value *= std::get<Number>(arg).value();
        }

        return Number{value};
    }

    Cell div(List const & args, Environment * env) {
        auto value = 1.0D;
        for (auto const & arg : args) {
            if (!arg.is_number()) {
                std::ostringstream msg{};
                msg << "Type error: Expected number got " << arg;

                throw std::runtime_error{msg.str()};
            }

            value /= std::get<Number>(arg).value();
        }

        return Number{value};
    }
}
