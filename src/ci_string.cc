#include "ci_string.hh"
#include <ostream>

namespace esquema {
    std::ostream & operator<<(std::ostream & ostr, CIString const & str) {
        return ostr << std::string_view(str.data(), str.size());
    }

    std::ostream & operator<<(std::ostream & ostr, CIStringView str) {
        return ostr << std::string_view(str.data(), str.size());
    }

    namespace literals::ci_string_literals {
        CIString operator""_cis(char const * str, std::size_t N) {
            return CIString{str, N};
        }
    }

    namespace literals::ci_string_view_literals {
            CIStringView operator""_cisv(char const * str, std::size_t N) {
                return CIStringView{str, N};
            }
    }
}
