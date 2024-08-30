#ifndef ESQUEMA_INTERP_HH_INCLUDED
#define ESQUEMA_INTERP_HH_INCLUDED

#include "environ.hh"
#include "parser.hh"
#include <unordered_map>

namespace esquema {
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
