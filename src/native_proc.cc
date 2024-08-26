#include "native_proc.hh"
#include "interp.hh"

#include <sstream>
#include <stdexcept>
#include <limits>

namespace {
    // TODO - I think I can do better than this. It was
    // really late at night and I wanted to finish so
    // this is what came out. Please don't judge me too
    // harshly.
    template <typename It, typename Op>
    esquema::Cell acc_op(It it, It last, double acc, Op op) {
        while (it != last) {
            auto const & cell = *it++;
            if (!cell.is_number()) {
                throw std::runtime_error{"Type error: expected number"};
            }

            acc = op(acc, std::get<esquema::Number>(cell).value());
        }

        return esquema::Number{acc};
    }

    template <typename It, typename Op>
    esquema::Cell map_rel_op(It it, It last, Op op) {
        auto prev = *it++;
        if (!prev.is_number()) {
            throw std::runtime_error{"Type error: expected number"};
        }

        while (it != last) {
            auto next = *it++;
            if (!next.is_number()) {
                throw std::runtime_error{"Type error: expected number"};
            }

            auto lhs = std::get<esquema::Number>(prev).value();
            auto rhs = std::get<esquema::Number>(next).value();
            if (!op(lhs, rhs)) {
                return esquema::Bool{false};
            }
            prev = next;
        }

        return esquema::Bool{true};
    }
}

namespace esquema {
    Cell add(List const & args, Environment * env) {
        if (args.empty()) {
            throw std::runtime_error{"Too few arguments, + needs at least two"};
        }

        return acc_op(
            args.begin(), args.end(), 
            0.0D, 
            [] (double lhs, double rhs) {
                return lhs + rhs;
            }
        ); 
    }

    Cell sub(List const & args, Environment * env) {
        if (args.size() < 2) {
            throw std::runtime_error{"Too few arguments: - requires at least two"};
        }

        return acc_op(
            args.begin(), args.end(),
            0.0D,
            [] (double lhs, double rhs) {
                return lhs - rhs;
            }
        );
    }

    Cell mul(List const & args, Environment * env) {
        if (args.size() < 2) {
            throw std::runtime_error{"Too few arguments: * requires at least two"};
        }

        return acc_op(
            args.begin(), args.end(),
            1.0D,
            [] (double lhs, double rhs) {
                return lhs * rhs;
            }
        );
    }

    // TODO - I think there's something wrong with this one
    // I know division is a tricky operation
    Cell div(List const & args, Environment * env) {
        if (args.size() < 2) {
            throw std::runtime_error{"Too few arguments: / requires at least two"};
        }

        return acc_op(
            args.begin(), args.end(),
            1.0D,
            [] (double lhs, double rhs) {
                if (rhs == 0.0D) {
                    throw std::runtime_error{"Zero division"};
                }
                return lhs / rhs;
            }
        );
    }

    Cell less(List const & args, Environment * env) {
        if (args.size() < 2) {
            throw std::runtime_error{"Too few arguments: < requires at least two"};
        }

        return map_rel_op(
            args.begin(), args.end(),
            [] (double lhs, double rhs) {
                return lhs < rhs;
            }
        );
    }

    Cell less_equal(List const & args, Environment * env) {
        if (args.size() < 2) {
            throw std::runtime_error{"Too few arguments: <= requires at least two"};
        }

        return map_rel_op(
            args.begin(), args.end(),
            [] (double lhs, double rhs) {
                return lhs <= rhs;
            }
        );
    }

    Cell greater(List const & args, Environment * env) {
        if (args.size() < 2) {
            throw std::runtime_error{"Too few arguments: > requires at least two"};
        }

        return map_rel_op(
            args.begin(), args.end(),
            [] (double lhs, double rhs) {
                return lhs > rhs;
            }
        );
    }

    Cell greater_equal(List const & args, Environment * env) {
        if (args.size() < 2) {
            throw std::runtime_error{"Too few arguments: >= requires at least two"};
        }

        return map_rel_op(
            args.begin(), args.end(),
            [] (double lhs, double rhs) {
                return lhs >= rhs;
            }
        );
    }

    // The definition of equivalence in Scheme is here:
    // https://conservatory.scheme.org/schemers/Documents/Standards/R5RS/HTML/r5rs-Z-H-9.html#%_sec_6.1
    Cell equal(List const & args, Environment * env) {
        if (args.size() != 2) {
            throw std::runtime_error{"eqv? takes exactly two arguments"};
        }

        auto it = args.begin();
        auto lhs = *it++;
        auto rhs = *it++;
        auto result = false;
        if (lhs.is_bool() && rhs.is_bool()) {
            result = std::get<Bool>(lhs).value() == std::get<Bool>(rhs).value();
        }

        // TODO - this needs to be case insensitive
        else if (lhs.is_symbol() && rhs.is_symbol()) {
            result = std::get<Symbol>(lhs).value() == std::get<Symbol>(rhs).value();
        }

        else if (lhs.is_nil() && rhs.is_nil()) {
            result = true;
        }

        else if (lhs.is_proc() && rhs.is_proc()) {
            result = &std::get<Proc>(lhs) == &std::get<Proc>(rhs);
        }

        return Bool{result};
    }

    // How Scheme coerces types into Booleans is detailed here
    // https://conservatory.scheme.org/schemers/Documents/Standards/R5RS/HTML/r5rs-Z-H-9.html#%_sec_6.3.1
    Cell negate(List const & args, Environment * env) {
        if (args.size() != 1) {
            throw std::runtime_error{"not? takes just one argument"};
        }

        auto rhs = args.front();
        auto result = false;
        if (rhs.is_bool()) {
            result = !std::get<Bool>(rhs).value();
        }

        return Bool{result};
    }                                       
}
