#ifndef ESQUEMA_PARSER_HH_INCLUDED
#define ESQUEMA_PARSER_HH_INCLUDED

#include "lexer.hh"
#include <optional>
#include <string>
#include <variant>
#include <list>

namespace esquema {
    class Symbol {
    public:
        bool operator==(std::string_view value) const noexcept;

    public:
        std::string_view strview() const noexcept;
        std::string const & str() const noexcept;

    public:
        explicit Symbol(std::string_view value);

    private:
        std::string m_value;
    };

    class Number {
    public:
        std::string_view strview() const noexcept;
        std::string const & str() const noexcept;

    public:
        explicit Number(std::string_view value);

    private:
        std::string m_value;
    };

    class Cell;
    using List = std::list<Cell>;
    
    using Proc = Cell(*)(List const &);

    class Nil {};

    class Cell : public std::variant<Nil, Symbol, Number, List, Proc> {
    public:
        bool is_nil() const noexcept;
        bool is_symbol() const noexcept;
        bool is_number() const noexcept;
        bool is_atom() const noexcept;
        bool is_list() const noexcept;
        bool is_proc() const noexcept;

    public:
        using variant::variant;
    };

    class Parser {
    public:
        Cell parse(std::string_view src);

    private:
        Cell parse_cell(Token & token);

    private:
        Lexer m_lexer;
    };
}

namespace std {
    template <>
    struct variant_size <esquema::Cell> 
        : variant_size <esquema::Cell::variant>
    { };

    template <size_t I>
    struct variant_alternative <I, esquema::Cell> 
        : variant_alternative <I, esquema::Cell::variant>
    { };
}

#endif
