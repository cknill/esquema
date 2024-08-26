#ifndef ESQUEMA_LEXER_HH_INCLUDED
#define ESQUEMA_LEXER_HH_INCLUDED

#include <iosfwd>
#include <optional>
#include <string>

namespace esquema {
    // A token designates the 'words' and 'punctuation' of a program
    // It will have a type and might have some text. Punctuation doesn't
    // have the corresponding text because it is constant
    class Token {
    public:
        enum class Type : unsigned char {
            LPar, RPar, Bool, Num, Id, Eof, Err = 255
        };

    // Friends
    public:
        friend std::ostream & operator<<(std::ostream & ostr, Type type);
        friend std::ostream & operator<<(std::ostream & ostr, Token const & token);

    // Operators
    public:
        bool operator==(Type type) const noexcept; 
        bool operator==(std::string_view txt) const noexcept;

    // Interface
    public:
        std::string_view strview() const noexcept;
        std::string const & str() const noexcept;
        Type type() const noexcept;

    // Constructors
    public:
        explicit Token(Type type, std::string_view txt = "");
        constexpr Token() noexcept
            : m_txt{}, m_type{Type::Err}
        { }

    // Details
    private:
        std::string m_txt;
        Type m_type;
    };

    // The Lexer holds a reference to the text we are interested
    // in turining into tokens. It knows the patterns corresponding
    // to the different types of tokens. It also knows how to track
    // the row and column of the text as it would appear in an editor.
    // The Lexer will not go over the edge of the buffer. Once it
    // reaches the end it will only return a token representing the
    // end of the input. Lexers are lightweight to copy so if you need
    // to do lookahead sort of analysis it is as easy as taking a copy
    // and going ahead and when the pattern fails reassign the stashed
    // Lexer.
    class Lexer {
    // Interface
    public:
        Token next();
        int row() const noexcept;
        int col() const noexcept;

    // Constructors
    public:
        explicit Lexer(std::string_view txt) noexcept;
        constexpr Lexer() noexcept
            : m_cursor{nullptr}, m_last{nullptr}
            , m_row{-1}, m_col{-1}
        { }

    // Helpers
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
        static bool is_bool(char c) noexcept;

    // Data
    private:
        char const * m_cursor;
        char const * m_last;
        int m_row, m_col;
    };

}

#endif
