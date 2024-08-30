#include "lexer.hh"
#include <ostream>

namespace esquema {
    std::ostream & operator<<(std::ostream & ostr, Token::Type type) {
        using enum Token::Type;
        switch (type) {
            case LPar: ostr << "LPar"; break;
            case RPar: ostr << "RPar"; break;
            case Num: ostr << "Num"; break;
            case Bool: ostr << "Bool"; break;
            case Id: ostr << "Id"; break;
            case Eof: ostr << "Eof"; break;
            default: ostr << "Err";
        }

        return ostr;
    }

    std::ostream & operator<<(std::ostream & ostr, Token const & token) {
        return ostr << token.m_type << ' ' << token.m_txt;
    }

    // Some convenience operators for Tokens
    bool Token::operator==(std::string_view txt) const noexcept {
        return m_txt == txt;
    }

    bool Token::operator==(Token::Type type) const noexcept {
        return m_type == type;
    }

    std::string_view Token::strview() const noexcept {
        return m_txt;
    }

    std::string const & Token::str() const noexcept {
        return m_txt;
    }

    Token::Type Token::type() const noexcept {
        return m_type;
    }

    Token::Token(Token::Type type, std::string_view txt) 
        : m_txt{txt}, m_type{type}
    { }
}
