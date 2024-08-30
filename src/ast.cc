#include "ast.hh"
#include <ostream>
#include <stdexcept>
#include <sstream>

namespace {
    using namespace esquema::literals::ci_string_view_literals;
}


namespace esquema {
    std::ostream & operator<<(std::ostream & ostr, Symbol const & sym) {
        return ostr << sym.m_value;
    }

    bool Symbol::operator==(CIStringView value) const noexcept {
        return m_value == value;
    }

    CIString const & Symbol::value() const noexcept {
        return m_value;
    }

    Symbol::Symbol(std::string_view value)
        : m_value{value.data(), value.size()}
    { }

    std::ostream & operator<<(std::ostream & ostr, Bool const & bool_) {
        if (bool_.m_value) {
            return ostr << "#t";
        }
        else {
            return ostr << "#f";
        }
    }

    bool Bool::value() const noexcept {
        return m_value;
    }

    Bool::Bool(bool value)
        : m_value{value}
    { }

    std::ostream & operator<<(std::ostream & ostr, Number const & num) {
        return ostr << num.m_value;
    }

    double Number::value() const noexcept {
        return m_value;
    }

    Number::Number(double value) 
        : m_value{value}
    { }

    std::ostream & operator<<(std::ostream & ostr, Nil) {
        return ostr << "Nil";
    }

    std::ostream & operator<<(std::ostream & ostr, Proc) {
        return ostr << "Proc";
    }

    std::ostream & operator<<(std::ostream & ostr, List const & list) {
        ostr << '(';
        if (list.empty()) {
            return ostr << ')';
        }

        else if (list.size() == 1) {
            return ostr << list.front() << ')';
        }

        else {
            auto last_elem = --list.end();
            for (auto it = std::begin(list); it != last_elem; ++it) {
                ostr << *it << ',';
            }

            return ostr << *last_elem << ')';
        }
    }

    std::ostream & operator<<(std::ostream & ostr, Cell const & cell) {
        if (auto ptr = std::get_if<Nil>(&cell)) {
            ostr << *ptr;
        }
        else if (auto ptr = std::get_if<Symbol>(&cell)) {
            ostr << *ptr;
        }
        else if (auto ptr = std::get_if<Bool>(&cell)) {
            ostr << *ptr;
        }
        else if (auto ptr = std::get_if<Number>(&cell)) {
            ostr << *ptr;
        }
        else if (auto ptr = std::get_if<List>(&cell)) {
            ostr << *ptr;
        }
        else if (auto ptr = std::get_if<Proc>(&cell)) {
            ostr << *ptr;
        }

        return ostr;
    }

    bool Cell::is_nil() const noexcept {
        return std::holds_alternative<Nil>(*this);
    }

    bool Cell::is_symbol() const noexcept {
        return std::holds_alternative<Symbol>(*this);
    }

    bool Cell::is_bool() const noexcept {
        return std::holds_alternative<Bool>(*this);
    }

    bool Cell::is_number() const noexcept {
        return std::holds_alternative<Number>(*this);
    }

    bool Cell::is_atom() const noexcept {
        return is_symbol() || is_bool() || is_number();
    }

    bool Cell::is_list() const noexcept {
        return std::holds_alternative<List>(*this);
    }

    bool Cell::is_proc() const noexcept {
        return std::holds_alternative<Proc>(*this);
    }
}
