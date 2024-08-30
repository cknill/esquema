#ifndef ESQUEMA_TOKEN_HH_INCLUDED
#define ESQUEMA_TOKEN_HH_INCLUDED

#include <cstdint>
#include <iosfwd>
#include <string>

namespace esquema {
    // A token designates the 'words' and 'punctuation' of a program
    // It will have a type and might have some text. Punctuation doesn't
    // have the corresponding text because it is constant
    class Token {
    public:
        enum class Type : std::uint8_t {
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
}

#endif
