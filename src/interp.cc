#include "interp.hh"
#include <sstream>
#include <stdexcept>

#include <iostream>

namespace {
    using namespace esquema::literals::ci_string_view_literals;
}

namespace esquema {
    Cell Interpreter::eval(std::string_view src) {
        return eval(m_parser.parse(src));
    }

    Cell Interpreter::eval(Cell const & cell) {
        // no need to evaluate just return them
        if (cell.is_nil() || cell.is_number() || cell.is_bool()) {
            return cell;
        }

        // the other atom does need to be resolved
        else if (cell.is_symbol()) {
            auto const & name = std::get<Symbol>(cell).value();
            auto it = m_env.find(name);
            if (it != m_env.end()) {
                return it->second;
            }

            else {
                std::ostringstream msg{};
                msg << "Dereferenced unbound variable '"
                    << name << "'";

                throw std::runtime_error{msg.str()};
            }
        }

        else if (cell.is_list()) {
            return eval(std::get<List>(cell));
        }

        else {
            return Nil{};
        }
    }

    Cell Interpreter::eval(List const & list) {
        if (list.empty()) {
            return list;
        }

        // First try to handle the special forms
        auto const & head = list.front();
        if (head.is_symbol()) {
            auto const & name = std::get<Symbol>(head).value();
            if (name == "define"_cisv) {
                if (list.size() != 3) {
                    throw std::runtime_error{"define requires two arguments"};
                }

                auto it = ++list.begin();
                auto var = *it++;
                if (!var.is_symbol()) {
                    throw std::runtime_error{"define requires a symbol to bind to"};
                }

                m_env.insert(std::get<Symbol>(var), eval(*it));
                return Nil{};
            }

            else if (name == "if"_cisv) {
                if (list.size() < 3) {
                    throw std::runtime_error{"if requires either two or three arguments"};
                }
                auto it = ++list.begin();
                auto cond = eval(*it++);
                if (!cond.is_bool()) {
                    throw std::runtime_error{"if condition must evaluate to boolean"};
                }

                auto true_path = *it++;
                auto false_path = Cell{};
                if (list.size() == 4) {
                    false_path = *it++;
                }

                if (std::get<Bool>(cond).value()) {
                    return eval(true_path);
                }

                else {
                    return eval(false_path);
                }
            }

            else if (name == "begin"_cisv) {
                Cell result;
                for (auto it = ++list.begin(); it != list.end(); ++it) {
                    result = eval(*it);
                }

                return result;
            }
        }

        // If we got here now we need to try and find
        // the proc in environment
        auto maybe_proc = eval(list.front());
        if (maybe_proc.is_proc()) {
            List args{};
            for (auto it = ++list.begin(); it != list.end(); ++ it) {
                args.push_back(eval(*it));
            }

            auto const & proc = std::get<Proc>(maybe_proc);
            return proc(args, &m_env);
        }

        throw std::runtime_error{"Not a procedure"};
    }

    // Make an interpreter with the default global environment
    Interpreter::Interpreter()
        : m_env{Environment::make_global()}
        , m_parser{}
    { }
}
