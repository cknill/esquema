#ifndef ESQUEMA_PARSER_HH_INCLUDED
#define ESQUEMA_PARSER_HH_INCLUDED

#include "lexer.hh"
#include <iosfwd>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace esquema {
    class Symbol {
    public:
        friend std::ostream & operator<<(std::ostream & ostr, Symbol const & sym);

    public:
        bool operator==(std::string_view value) const noexcept;

    public:
        std::string const & value() const noexcept;

    public:
        explicit Symbol(std::string_view value);

    private:
        std::string m_value;
    };

    class Bool {
    public:
        friend std::ostream & operator<<(std::ostream & ostr, Bool const & bool_);

    public:
        bool value() const noexcept;

    public:
        explicit Bool(std::string_view value);
        explicit Bool(bool value);

    private:
        bool m_value;
    };

    class Number {
    public:
        friend std::ostream & operator<<(std::ostream & ostr, Number const & num);

    public:
        double value() const noexcept;

    public:
        explicit Number(double value);

    private:
        double m_value;
    };

    class Cell;
    using List = std::list<Cell>;
    std::ostream & operator<<(std::ostream & ostr, List const & list);


    class Environment;
    using Proc = Cell(*)(List const &, Environment *);
    std::ostream & operator<<(std::ostream & ostr, Proc);

    class Nil {};
    std::ostream & operator<<(std::ostream & ostr, Nil);

    class Cell : public std::variant<Nil, Symbol, Bool, Number, List, Proc> {
    public:
        friend std::ostream & operator<<(std::ostream & ostr, Cell const & cell);

    public:
        bool is_nil() const noexcept;
        bool is_symbol() const noexcept;
        bool is_number() const noexcept;
        bool is_bool() const noexcept;
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
