#include "parser.hh"
#include <ostream>
#include <stdexcept>
#include <sstream>

namespace {
    using namespace std::literals::string_view_literals;
}

namespace esquema {
    Cell Parser::parse(std::string_view src) {
        m_lexer = Lexer{src};
        // Just in case the string is empty. I take
        // care of this in main.cc
        if (src.empty()) {
            return Nil{};
        }

        auto cur = m_lexer.next();
        auto result = parse_cell(cur);
        if (cur != Token::Type::Eof) {
            std::ostringstream msg{};
            msg << "Malformed expression near "
                << m_lexer.row() << '-' << m_lexer.col();

            throw std::runtime_error{msg.str()};
        }

        return result;
    }

    Cell Parser::parse_cell(Token & cur) {
        if (cur == Token::Type::LPar) {
            List list{};
            cur = m_lexer.next();
            while (cur != Token::Type::RPar) {
                if (cur == Token::Type::Eof) {
                    std::ostringstream msg{};
                    msg << "Unexpected EOF near "sv
                        << m_lexer.row() << '-' << m_lexer.col();

                    throw std::runtime_error{msg.str()};
                }

                list.push_back(parse_cell(cur));
            }

            cur = m_lexer.next();
            return std::move(list);
        }

        // Just in case we come across a wayward ')'
        // the function above consumes any matched parens
        else if (cur == Token::Type::RPar) {
            std::ostringstream msg{};
            msg << "Unexpected ')' near "sv
                << m_lexer.row() << '-' << m_lexer.col();

            throw std::runtime_error{msg.str()};
        }

        // Do the conversion here to take advantage
        // of row and column information
        else if (cur == Token::Type::Num) {
            auto value = 0.0D;
            try {
                value = std::stod(cur.str());
            }

            catch (std::exception const & ex) {
                std::ostringstream msg{};
                msg << "Invalid number '"sv << cur.strview()
                    << "' near "sv << m_lexer.row() << '-'
                    << m_lexer.col();

                throw std::runtime_error{msg.str()};
            }

            auto atom = Number{value};
            cur = m_lexer.next();
            return std::move(atom);
        }

        else if (cur == Token::Type::Bool) {
            auto atom = Bool{cur.strview()};
            cur = m_lexer.next();
            
            return std::move(atom);
        }

        else if (cur == Token::Type::Id) {
            auto atom = Symbol{cur.strview()};
            cur = m_lexer.next();

            return std::move(atom);
        }

        else {
            std::ostringstream msg{};
            msg << "Unexpected token '"sv << cur.type() << "' near "sv
                << m_lexer.row() << '-' << m_lexer.col();
            throw std::runtime_error{msg.str()};
        }
    }

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

    Bool::Bool(std::string_view value)
        : m_value{false}
    {
        if (value == "#t" || value == "#T") {
            m_value = true;
        }
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
        return ostr << "Nil"sv;
    }

    std::ostream & operator<<(std::ostream & ostr, Proc) {
        return ostr << "Proc"sv;
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
