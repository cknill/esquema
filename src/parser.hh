#ifndef ESQUEMA_PARSER_HH_INCLUDED
#define ESQUEMA_PARSER_HH_INCLUDED

#include "ci_string.hh"
#include "lexer.hh"
#include <iosfwd>
#include <list>
#include <memory>
#include <optional>
#include <variant>

namespace esquema {
    // A Symbol in Scheme can bind to a value or a procedure that
    // you later can call. For right now I only support values.
    class Symbol {
    // Friends
    public:
        friend std::ostream & operator<<(std::ostream & ostr, Symbol const & sym);

    // Operators
    public:
        bool operator==(CIStringView value) const noexcept;

    // Interface
    public:
        CIString const & value() const noexcept;

    // Constructors
    public:
        explicit Symbol(std::string_view value);

    // Data
    private:
        CIString m_value;
    };

    // Bool in Scheme is any of: #t, #T, #f, #F. Other
    // values can also evaluate to #t.
    class Bool {
    // Friends
    public:
        friend std::ostream & operator<<(std::ostream & ostr, Bool const & bool_);

    // Interface
    public:
        bool value() const noexcept;

    // Constructors
    public:
        explicit Bool(std::string_view value);
        explicit Bool(bool value);

    // Data
    private:
        bool m_value;
    };

    // Numbers in scheme all go to floating point

    // TODO - It'd be nice to support bigint and unlimited
    // precision numbers, but I'm not doing that right now.
    class Number {
    // Friends
    public:
        friend std::ostream & operator<<(std::ostream & ostr, Number const & num);

    // Interface
    public:
        double value() const noexcept;

    // Constructors
    public:
        explicit Number(double value);

    // Data
    private:
        double m_value;
    };

    // Forward declare this to get out of a tight situation
    class Cell;

    // I use list because there can be lists of lists of lists
    // and when a vector reallocates that would be a beast
    // so list it is.

    // TODO - maybe this could be forward_list instead of a doublely
    // linked list
    using List = std::list<Cell>;
    std::ostream & operator<<(std::ostream & ostr, List const & list);


    // Also forward declare this to avoid another tight situation
    class Environment;

    // A proc is just a function pointer for right now.

    // TODO - I could change this to std::function and
    // do all sorts of stateful things
    using Proc = Cell(*)(List const &, Environment *);
    std::ostream & operator<<(std::ostream & ostr, Proc);

    // Represents nothing at all, some operations return it
    class Nil {};
    std::ostream & operator<<(std::ostream & ostr, Nil);

    // The any value in Scheme, in this case it is a variant
    // of any of the above. I inherit from std::variant for
    // all the nice constructors that the stdlib implementators
    // wrote for my benefit. Further down I extend namespace
    // std to allow for the variant non-member functions to work
    class Cell : public std::variant<Nil, Symbol, Bool, Number, List, Proc> {
    // Friends
    public:
        friend std::ostream & operator<<(std::ostream & ostr, Cell const & cell);

    // Interface
    public:
        bool is_nil() const noexcept;
        bool is_symbol() const noexcept;
        bool is_number() const noexcept;
        bool is_bool() const noexcept;
        bool is_atom() const noexcept;
        bool is_list() const noexcept;
        bool is_proc() const noexcept;

    // Constructors
    public:
        using variant::variant;
    };

    // A parser uses a lexer to produce an abstract syntax tree
    // analysis of a program in a string. Since Lexers are cheap
    // to copy so are parsers which allows you to do lookahead parsing
    // fairly simply, and cheaply.
    class Parser {
    public:
        Cell parse(std::string_view src);

    private:
        Cell parse_cell(Token & token);

    private:
        Lexer m_lexer;
    };
}

// I do this to be able to use get, holds_alternative, and the 
// other non-member functions of std::variant. The stdlib
// implementators welcome template specializations in namespace
// std
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
