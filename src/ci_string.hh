#ifndef ESQUEMA_CI_STRING_HH_INCLUDED
#define ESQUEMA_CI_STRING_HH_INCLUDED

#include <cstdint>
#include <cctype>
#include <iosfwd>
#include <string>

namespace esquema {
    struct ci_char_traits : std::char_traits<char> {
    public:
        static constexpr char toupper(char c) noexcept {
            return std::toupper((unsigned char)c);
        }

        static constexpr bool eq(char lhs, char rhs) noexcept {
            return toupper(lhs) == toupper(rhs);
        }

        static constexpr bool lt(char lhs, char rhs) noexcept {
            return toupper(lhs) < toupper(rhs);
        }

        static constexpr int compare(char const * lhs, char const * rhs, std::size_t N) {
            while (N-- != 0) {
                if (toupper(*lhs) < toupper(*rhs)) {
                    return -1;
                }

                else if (toupper(*rhs) < toupper(*lhs)) {
                    return 1;
                }

                else {
                    ++lhs;
                    ++rhs;
                }
            }

            return 0;
        }

        static constexpr char const * find(char const * str, std::size_t N, char  c) {
            auto const uc{toupper(c)};
            while (N-- != 0) {
                if (toupper(*str) == uc) {
                    return str;
                }

                else {
                    ++str;
                }
            }

            return nullptr;
        }
    };

    template <typename SrcTraits, typename DstTraits>
    constexpr std::basic_string_view<char, DstTraits> 
        traits_cast(std::basic_string_view<char, SrcTraits> src) noexcept 
    {
        return std::basic_string_view<char, DstTraits>(src.data(), src.size());    
    }

    using CIString = std::basic_string<char, ci_char_traits>;
    using CIStringView = std::basic_string_view<char, ci_char_traits>;

    std::ostream & operator<<(std::ostream & ostr, CIString const & str);
    std::ostream & operator<(std::ostream & ostr, CIStringView str);

    namespace literals::ci_string_literals {
        CIString operator""_cis(char const * str, std::size_t N); 
    }

    namespace literals::ci_string_view_literals {
        CIStringView operator""_cisv(char const * str, std::size_t N);
    }
}

namespace std {
    template <>
    struct hash <esquema::CIString> {
    public:
        size_t operator()(esquema::CIString const & str) const {
            // see djb2 section here http://www.cse.yorku.ca/~oz/hash.html
            uint64_t hash = 5381;
            for (auto c : str) {
                const auto uc{esquema::ci_char_traits::toupper(c)};
                hash = ((hash << 5) + hash) + uc; 
            }

            return hash;
        }
    };

    template <>
    struct hash <esquema::CIStringView> {
    public:
        size_t operator()(esquema::CIStringView str) const {
            // see djb2 section here http://www.cse.yorku.ca/~oz/hash.html
            uint64_t hash = 5381;
            for (auto c : str) {
                const auto uc{esquema::ci_char_traits::toupper(c)};
                hash = ((hash << 5) + hash) + uc; 
            }

            return hash;
        }
    };
}

#endif
