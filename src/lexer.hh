#ifndef ESQUEMA_LEXER_HH_INCLUDED
#define ESQUEMA_LEXER_HH_INCLUDED

#include <iosfwd>
#include <optional>
#include <string>

namespace esquema {
    class Token {
    public:
        enum class Type : unsigned char {
            LPar, RPar, Num, Id, Eof, Err = 255
        };

    public:
        friend std::ostream & operator<<(std::ostream & ostr, Type type);
        friend std::ostream & operator<<(std::ostream & ostr, Token const & token);

    public:
        bool operator==(Type type) const noexcept; 
        bool operator==(std::string_view txt) const noexcept;

    public:
        std::string_view strview() const noexcept;
        std::string const & str() const noexcept;
        Type type() const noexcept;

    public:
        explicit Token(Type type, std::string_view txt = "");
        constexpr Token() noexcept
            : m_txt{}, m_type{Type::Err}
        { }

    private:
        std::string m_txt;
        Type m_type;
    };

    class Lexer {
    public:
        Token next();
        int row() const noexcept;
        int col() const noexcept;

    public:
        explicit Lexer(std::string_view txt) noexcept;
        constexpr Lexer() noexcept
            : m_cursor{nullptr}, m_last{nullptr}
            , m_row{-1}, m_col{-1}
        { }

    private:
        bool is_eof() const noexcept;
        std::optional<char> peek() const noexcept;
        void advance() noexcept;
        template <typename P>
        void advance_while(P && pred) noexcept {
            while (true) {
                auto cur = peek();
                if (!(cur && pred(*cur))) {
                    break;
                }

                advance();
            }
        }

    private:
        void consume_ws() noexcept;
        void consume_comment() noexcept;
        Token scan_id();
        Token scan_number();
        Token scan_symbol();

    private:
        static bool is_initial_id(char c) noexcept;
        static bool is_subsequent_id(char c) noexcept;
        static bool is_number(char c) noexcept;

    private:
        char const * m_cursor;
        char const * m_last;
        int m_row, m_col;
    };

}

#endif
