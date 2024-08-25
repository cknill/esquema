#include "lexer.hh"
#include <stdexcept>
#include <sstream>

namespace esquema {
    Token Lexer::next() {
        auto cur = peek();
        if (!cur) {
            return Token{Token::Type::Eof};
        }

        if (std::isspace(*cur)) {
            consume_ws();
            if (is_eof()) {
                return Token{Token::Type::Eof};
            }

            cur = peek();
        }

        if (is_initial_id(*cur)) {
            return scan_id();
        }

        else if (is_number(*cur)) {
            return scan_number();
        }

        else {
            return scan_symbol(); 
        }
    }

    int Lexer::row() const noexcept {
        return m_row;
    }

    int Lexer::col() const noexcept {
        return m_col;
    }

    Token Lexer::scan_id() {
        auto anchor = m_cursor;
        advance();
        advance_while(is_subsequent_id);
        return Token{Token::Type::Id, std::string_view(anchor, m_cursor)};
    }

    Token Lexer::scan_number() {
        auto anchor = m_cursor;
        auto c = peek();
        auto tmp = *this;
        switch (*c) {
            case '+': case '-': case '.':
                advance();
                c = peek();
                if (!(c && (std::isdigit(*c) || *c == '.'))) {
                    *this = tmp;
                    return scan_id();
                }

                [[fallthrough]];
            default:
                advance();
                advance_while([] (char c) { return std::isdigit(c); });
                c = peek();
                if (c && *c == '.') {
                    advance();
                    advance_while([] (char c) { return std::isdigit(c); });
                }
        }

        return Token{Token::Type::Num, std::string_view(anchor, m_cursor)};
    }

    Token Lexer::scan_symbol() {
        auto c = peek();
        if (*c == ';') {
            consume_comment();
            c = peek();
            if (!c) {
                return Token{Token::Type::Eof};
            }
        }

        if (*c == '(') {
            advance();
            return Token{Token::Type::LPar};
        }

        else if (*c == ')') {
            advance();
            return Token{Token::Type::RPar};
        }

        else {
            std::ostringstream msg{};
            msg << "Encountered unknown character '"
                << *c << "' near " << m_row << '-' 
                << m_col;

            throw std::runtime_error{msg.str()};
        }
    }

    bool Lexer::is_initial_id(char c) noexcept {
        return std::isalpha(c) ||
               c == '!' || c == '$' || c == '%' || c == '&' ||
               c == '*' || c == '/' || c == ':' || c == '<' || 
               c == '=' || c == '>' || c == '?' || c == '~' || 
               c == '_' || c == '^';
    }

    bool Lexer::is_subsequent_id(char c) noexcept {
        return std::isalnum(c) ||
               c == '!' || c == '$' || c == '%' || c == '&' ||
               c == '*' || c == '/' || c == ':' || c == '<' ||
               c == '=' || c == '>' || c == '?' || c == '~' || 
               c == '_' || c == '^' || c == '.' || c == '+' || 
               c == '-';
    }

    bool Lexer::is_number(char c) noexcept {
        return std::isdigit(c) || c == '+' || c == '-' || c == '.';
    }

    void Lexer::consume_ws() noexcept {
        // We've already seen a ws character;
        advance();
        advance_while([](char c){ return std::isspace(c); });
    }

    void Lexer::consume_comment() noexcept {
        // We've already seen the semicolon
        advance();
        advance_while([](char c) { return c != '\n'; });
    }

    bool Lexer::is_eof() const noexcept {
        return m_cursor == m_last;
    }

    std::optional<char> Lexer::peek() const noexcept {
        if (!is_eof()) {
            return std::optional{*m_cursor};
        }

        else {
            return {};
        }
    }

    void Lexer::advance() noexcept {
        if (!is_eof()) {
            if (auto c = peek(); c && *c == '\n') {
                ++m_row;
                m_col = 1;
            }
            else {
                ++m_col;
            }

            ++m_cursor;
        }
    }

    Lexer::Lexer(std::string_view txt) noexcept
        : m_cursor{nullptr}, m_last{nullptr}
        , m_row{-1}, m_col{-1}
    {
        if (!txt.empty()) {
            m_cursor = txt.data();
            m_last = txt.data() + txt.size();
            m_row = m_col = 1;
        }
    }

    std::ostream & operator<<(std::ostream & ostr, Token::Type type) {
        using enum Token::Type;
        switch (type) {
            case LPar: ostr << "LPar"; break;
            case RPar: ostr << "RPar"; break;
            case Num: ostr << "Num"; break;
            case Id: ostr << "Id"; break;
            case Eof: ostr << "Eof"; break;
            default: ostr << "Err";
        }

        return ostr;
    }

    std::ostream & operator<<(std::ostream & ostr, Token const & token) {
        return ostr << token.m_type << ' ' << token.m_txt;
    }

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
