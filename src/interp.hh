#ifndef ESQUEMA_INTERP_HH_INCLUDED
#define ESQUEMA_INTERP_HH_INCLUDED

#include "parser.hh"
#include <memory>
#include <unordered_map>

namespace esquema {
    class Environment {
    private:
        using container_type = std::unordered_map<
            std::string, std::shared_ptr<Cell>
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
        iterator insert(Symbol const & symbol, Cell const & cell);

    public:
        Environment(List const & args, List const & params, std::shared_ptr<Environment> outer);
        explicit Environment(std::shared_ptr<Environment> outer = nullptr);

    private:
        std::unordered_map<std::string, std::shared_ptr<Cell>> m_inner;
        std::shared_ptr<Environment> m_outer;
    };

    class Interpreter {
    public:
        Cell eval(std::string_view src);

    public:
        Interpreter();

    private:
        Environment m_env;
        Parser m_parser;
    };
}

#endif
