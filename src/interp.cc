#include "interp.hh"
#include <numbers>

namespace {
    using namespace std::literals::string_literals;
}

namespace esquema {
    Cell Interpreter::eval(std::string_view src) {
        auto cells = m_parser.parse(src);
        return Nil{};
    }

    Interpreter::Interpreter()
        : m_env{Environment::make_global()}
        , m_parser{}
    { }

    Environment Environment::make_global() {
        auto env = Environment{};
        using env_ptr = std::shared_ptr<Environment>;
        env.m_inner = {{
            { "pi"s, std::make_shared<Cell>(nullptr) },
            { "e"s, std::make_shared<Cell>(nullptr) },
        }};

        return env;
    }

    using iterator = Environment::iterator;
    iterator Environment::begin() noexcept {
        return m_inner.begin();
    }

    iterator Environment::end() noexcept {
        return m_inner.end();
    }

    using const_iterator = Environment::const_iterator;
    const_iterator Environment::begin() const noexcept {
        return m_inner.begin();
    }

    const_iterator Environment::end() const noexcept {
        return m_inner.end();
    }

    const_iterator Environment::find(Symbol const & sym) const noexcept {
        auto it = m_inner.find(sym.value());
        if (it == std::end(m_inner) && m_outer) {
           it = m_outer->find(sym); 
        }

        return it;
    }

    iterator Environment::insert(Symbol const & sym, Cell const & cell) {
        auto ptr = std::make_shared<Cell>(cell);
        auto [it, inserted] = m_inner.insert({sym.value(), ptr});
        if (!inserted) {
            it->second = std::move(ptr);
        }

        return it;
    }

    Environment::Environment(List const & syms, List const & cells, std::shared_ptr<Environment> outer)
        : m_inner{}, m_outer{std::move(outer)}
    {
        auto cell_it = std::begin(cells);
        for (auto sym_it = std::begin(syms); sym_it != std::end(syms); ++sym_it) {
            if (!sym_it->is_symbol()) {
                throw std::runtime_error{"Expected a symbol"};
            }

            m_inner.emplace(
                std::get<Symbol>(*sym_it).value(), 
                std::make_shared<Cell>(*cell_it++)
            );
        }
    }

    Environment::Environment(std::shared_ptr<Environment> outer)
        : m_inner{}, m_outer{std::move(outer)}
    { }
}
