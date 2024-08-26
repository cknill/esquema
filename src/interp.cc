#include "interp.hh"
#include "native_proc.hh"
#include <numbers>
#include <sstream>
#include <stdexcept>

#include <iostream>

namespace {
    using namespace std::literals::string_literals;
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
            if (name == "define") {
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

            else if (name == "if") {
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

            else if (name == "begin") {
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

    Interpreter::Interpreter()
        : m_env{Environment::make_global()}
        , m_parser{}
    { }

    Environment Environment::make_global() {
        auto env = Environment{};
        env.m_inner = {{
            { "+"s, add }, { "-"s, sub }, 
            { "*"s, mul }, { "/"s, div },
            { "<"s, less }, { "<="s, less_equal }, 
            { ">"s, greater }, { ">="s, greater_equal },
            { "eqv?"s, equal }, { "not", negate }, 
            { "pi"s, Cell{Number{std::numbers::pi}} },
            { "e"s, Cell{Number{std::numbers::e}} },
        }};

        return env;
    }

    Environment::iterator Environment::begin() noexcept {
        return m_inner.begin();
    }

    Environment::iterator Environment::end() noexcept {
        return m_inner.end();
    }

    Environment::const_iterator Environment::begin() const noexcept {
        return m_inner.begin();
    }

    Environment::const_iterator Environment::end() const noexcept {
        return m_inner.end();
    }

    Environment::const_iterator Environment::find(Symbol const & sym) const noexcept {
        return find(sym.value());
    }

    Environment::const_iterator Environment::find(std::string const & name) const noexcept {
        auto it = m_inner.find(name);
        if (it == std::end(m_inner) && m_outer) {
            it = m_outer->find(name);
        }

        return it;
    }

    Environment::iterator Environment::insert(Symbol const & sym, Cell const & cell) {
        auto [it, inserted] = m_inner.insert({sym.value(), cell});
        if (!inserted) {
            it->second = cell;
        }

        return it;
    }

    Environment::Environment(Environment * outer)
        : m_inner{}, m_outer{outer}
    { }
}
