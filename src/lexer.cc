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

        else if (*cur == ';') {
            consume_comment();
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

    // See https://conservatory.scheme.org/schemers/Documents/Standards/R5RS/HTML/r5rs-Z-H-5.html#%_sec_2.1
    // for the definition of an identifier
    Token Lexer::scan_id() {
        auto anchor = m_cursor;
        advance();
        advance_while(is_subsequent_id);
        return Token{Token::Type::Id, std::string_view(anchor, m_cursor)};
    }

    // Partial implementation of Scheme's numbers
    // recognizes the following: (+/-)?123, (+/-)?0.123, (+/-)?.123
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

    // TODO - rename this member function because I completely forgot that
    // symbols are a thing in scheme. Rename to scan_punc or something like
    // that. 

    // Scans characters that aren't initially part of a number or id
    // throws if the character isn't recognized.
    Token Lexer::scan_symbol() {
        auto c = peek();
        if (*c == '(') {
            advance();
            return Token{Token::Type::LPar};
        }

        else if (*c == ')') {
            advance();
            return Token{Token::Type::RPar};
        }

        else if (*c == '#') {
            auto const * anchor = m_cursor;
            auto row = m_row;
            auto col = m_col;
            advance();
            c = peek();
            if (c && is_bool(*c)) {
                advance();
                return Token{Token::Type::Bool, std::string_view(anchor, m_cursor)};
            }

            std::ostringstream msg{};
            msg << "Encountered unknown character '#' near "
                << row << '-' << col;
            
            throw std::runtime_error{msg.str()};
        }

        else {
            std::ostringstream msg{};
            msg << "Encountered unknown character '"
                << *c << "' near " << m_row << '-' 
                << m_col;

            throw std::runtime_error{msg.str()};
        }
    }

    // Tests if the character can be the start of an identifier
    bool Lexer::is_initial_id(char c) noexcept {
        return std::isalpha(c) ||
               c == '!' || c == '$' || c == '%' || c == '&' ||
               c == '*' || c == '/' || c == ':' || c == '<' || 
               c == '=' || c == '>' || c == '?' || c == '~' || 
               c == '_' || c == '^';
    }

    // Tests if the charcter can be the subsequent letters
    // of an identifier
    bool Lexer::is_subsequent_id(char c) noexcept {
        return std::isalnum(c) ||
               c == '!' || c == '$' || c == '%' || c == '&' ||
               c == '*' || c == '/' || c == ':' || c == '<' ||
               c == '=' || c == '>' || c == '?' || c == '~' || 
               c == '_' || c == '^' || c == '.' || c == '+' || 
               c == '-';
    }

    // Tests if the character can start a number
    bool Lexer::is_number(char c) noexcept {
        return std::isdigit(c) || c == '+' || c == '-' || c == '.';
    }

    // These are the valid characters for a boolean
    bool Lexer::is_bool(char c) noexcept {
        switch (c) {
            case 't': case 'T': case 'f': case 'F':
                return true;
            default:
                return false;
        }
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

    // May shift the buffer pointer as long as there is
    // runway left. It also does row and column bookkeeping
    // No attempt is made to handle Windows line endings.
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
}
