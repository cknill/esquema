#include "parser.hh"
#include <stdexcept>
#include <sstream>

namespace esquema {
    Cell Parser::parse(std::string_view src) {
        auto cells = List{};
        m_lexer = Lexer{src};
        auto cur = m_lexer.next();
        return parse_cell(cur);
    }

    Cell Parser::parse_cell(Token & cur) {
        if (cur == Token::Type::LPar) {
            List list{};
            cur = m_lexer.next();
            while (cur != Token::Type::RPar) {
                if (cur == Token::Type::Eof) {
                    std::ostringstream msg{};
                    msg << "Unexpected EOF near "
                        << m_lexer.row() << '-' << m_lexer.col();

                    throw std::runtime_error{msg.str()};
                }

                list.push_back(parse_cell(cur));
            }

            cur = m_lexer.next();
            return std::move(list);
        }

        else if (cur == Token::Type::RPar) {
            std::ostringstream msg{};
            msg << "Unexpected ')' near "
                << m_lexer.row() << '-' << m_lexer.col();

            throw std::runtime_error{msg.str()};
        }

        else if (cur == Token::Type::Num) {
            auto atom = Number{cur.strview()};
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
            msg << "Unexpected token '" << cur.type() << "' near "
                << m_lexer.row() << '-' << m_lexer.col();
            throw std::runtime_error{msg.str()};
        }
    }

    bool Symbol::operator==(std::string_view value) const noexcept {
        return m_value == value;
    }

    std::string_view Symbol::strview() const noexcept {
        return m_value;
    }

    std::string const & Symbol::str() const noexcept {
        return m_value;
    }

    Symbol::Symbol(std::string_view value)
        : m_value{value}
    { }

    std::string_view Number::strview() const noexcept {
        return m_value;
    }

    std::string const & Number::str() const noexcept {
        return m_value;
    }

    Number::Number(std::string_view value) 
        : m_value{value}
    { }

    bool Cell::is_nil() const noexcept {
        return std::holds_alternative<Nil>(*this);
    }

    bool Cell::is_symbol() const noexcept {
        return std::holds_alternative<Symbol>(*this);
    }

    bool Cell::is_number() const noexcept {
        return std::holds_alternative<Number>(*this);
    }

    bool Cell::is_atom() const noexcept {
        return is_symbol() || is_number();
    }

    bool Cell::is_list() const noexcept {
        return std::holds_alternative<List>(*this);
    }

    bool Cell::is_proc() const noexcept {
        return std::holds_alternative<Proc>(*this);
    }
}
