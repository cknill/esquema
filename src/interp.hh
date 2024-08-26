#ifndef ESQUEMA_INTERP_HH_INCLUDED
#define ESQUEMA_INTERP_HH_INCLUDED

#include "parser.hh"
#include <unordered_map>

namespace esquema {
    // The Environment holds Symbols that have been
    // bound to a value. A user can add further bindings.
    // Esquema also provides a default global environment
    // with mathmatical constants and operations. The Environment
    // keeps a non-owning pointer to its parent environment
    // and if it can't find a symbol it searches each enclosing
    // environment until it hits the top or finds the symbol.

    // TODO - All this is wildly inefficient. Everything
    // is passed by copy and this will break down with
    // closures, list, and procedures. To support all
    // that GC must be implemented.
    class Environment {
    // Save myself some typing but the user doesn't need
    // this so keep it private
    private:
        using container_type = std::unordered_map<
            std::string, Cell
        >;

    // Interface
    public:
        static Environment make_global();

    // Iterator interface
    public:
        using iterator = container_type::iterator;
        iterator begin() noexcept;
        iterator end() noexcept;

        using const_iterator = container_type::const_iterator;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;

    // More Interface
    public:
        const_iterator find(Symbol const & symbol) const noexcept;
        const_iterator find(std::string const & name) const noexcept;

        // Care must be taken with this as it can prevent the lookup
        // of symbols in enclonsing Environments if they have the
        // same name. That may well be desired, but be warned.
        iterator insert(Symbol const & symbol, Cell const & cell);

    // Constructor
    public:
        explicit Environment(Environment * outer = nullptr);

    /// Data
    private:
        // Just in case we've forgotten, this is an unordered_map
        // with string keys and Cell values.
        container_type m_inner;

        // This is a non-owning pointer for now. It may make
        // sense to turn this into a shared_ptr or something
        // else more exotic when closures are a thing.
        Environment * m_outer;
    };


    // The interpreter holds a parser and an environment.
    // It has the parser produce an abstract syntax tree
    // from the string you give it.
    // It knowns how to traverses the abstract syntax tree 
    // and how to evaluate each node, producing a result value
    // at the very end. In Scheme atoms evaluate to themselves and
    // that is how we bottom out of the recursive function.
    class Interpreter {
    // Interface
    public:
        Cell eval(std::string_view src);

    // Constructor
    public:
        Interpreter();

    // Helpers
    private:
        Cell eval(Cell const & cell);
        Cell eval(List const & list);

    // Data
    private:
        Environment m_env;
        Parser m_parser;
    };
}

#endif
