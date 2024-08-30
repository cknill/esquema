#ifndef ESQUEMA_PARSER_HH_INCLUDED
#define ESQUEMA_PARSER_HH_INCLUDED

#include "lexer.hh"
#include "ast.hh"

namespace esquema {
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

#endif
