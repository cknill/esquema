#ifndef ESQUEMA_INTERP_HH_INCLUDED
#define ESQUEMA_INTERP_HH_INCLUDED

#include "parser.hh"
#include <memory>
#include <unordered_map>

namespace esquema {
    class Environment {
    private:
        using container_type = std::unordered_map<
            std::string, Cell
        >;

    public:
        static Environment make_global();

    public:
        using iterator = container_type::iterator;
        iterator begin() noexcept;
        iterator end() noexcept;

        using const_iterator = container_type::const_iterator;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;

    public:
        const_iterator find(Symbol const & symbol) const noexcept;
        const_iterator find(std::string const & name) const noexcept;
        iterator insert(Symbol const & symbol, Cell const & cell);

    public:
        explicit Environment(Environment * outer = nullptr);

    private:
        container_type m_inner;
        Environment * m_outer;
    };

    class Interpreter {
    public:
        Cell eval(std::string_view src);

    public:
        Interpreter();

    private:
        Cell eval(Cell const & cell);
        Cell eval(List const & list);

    private:
        Environment m_env;
        Parser m_parser;
    };
}

#endif
