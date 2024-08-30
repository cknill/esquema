#include "environ.hh"
#include "native_proc.hh"
#include <numbers>

namespace {
    using namespace esquema::literals::ci_string_literals;
}

namespace esquema {
    // The global environment in all its glory
    Environment Environment::make_global() {
        auto env = Environment{};
        env.m_inner = {{
            { "+"_cis, add }, { "-"_cis, sub }, 
            { "*"_cis, mul }, { "/"_cis, div },
            { "<"_cis, less }, { "<="_cis, less_equal }, 
            { ">"_cis, greater }, { ">="_cis, greater_equal },
            { "eqv?"_cis, equal }, { "not"_cis, negate }, 
            { "pi"_cis, Cell{Number{std::numbers::pi}} },
            { "e"_cis, Cell{Number{std::numbers::e}} },
        }};

        return env;
    }

    Environment::iterator Environment::begin() noexcept {
        return m_inner.begin();
    }

    Environment::iterator Environment::end() noexcept {
        return m_inner.end();
    }

    Environment::const_iterator Environment::begin() const noexcept {
        return m_inner.begin();
    }

    Environment::const_iterator Environment::end() const noexcept {
        return m_inner.end();
    }

    Environment::const_iterator Environment::find(Symbol const & sym) const noexcept {
        return find(sym.value());
    }

    Environment::const_iterator Environment::find(CIString const & name) const noexcept {
        auto it = m_inner.find(name);
        if (it == std::end(m_inner) && m_outer) {
            it = m_outer->find(name);
        }

        return it;
    }

    Environment::iterator Environment::insert(Symbol const & sym, Cell const & cell) {
        auto [it, inserted] = m_inner.insert({sym.value(), cell});
        if (!inserted) {
            it->second = cell;
        }

        return it;
    }

    Environment::Environment(Environment * outer)
        : m_inner{}, m_outer{outer}
    { }
}
