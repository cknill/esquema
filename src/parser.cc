#include "parser.hh"
#include <stdexcept>
#include <sstream>

namespace {
    using namespace std::literals::string_view_literals;
    using namespace esquema::literals::ci_string_view_literals;
}

namespace esquema {
    Cell Parser::parse(std::string_view src) {
        m_lexer = Lexer{src};
        // Just in case the string is empty. I take
        // care of this in main.cc
        if (src.empty()) {
            return Nil{};
        }

        auto cur = m_lexer.next();
        auto result = parse_cell(cur);
        if (cur != Token::Type::Eof) {
            std::ostringstream msg{};
            msg << "Malformed expression near "
                << m_lexer.row() << '-' << m_lexer.col();

            throw std::runtime_error{msg.str()};
        }

        return result;
    }

    Cell Parser::parse_cell(Token & cur) {
        if (cur == Token::Type::LPar) {
            List list{};
            cur = m_lexer.next();
            while (cur != Token::Type::RPar) {
                if (cur == Token::Type::Eof) {
                    std::ostringstream msg{};
                    msg << "Unexpected EOF near "sv
                        << m_lexer.row() << '-' << m_lexer.col();

                    throw std::runtime_error{msg.str()};
                }

                list.push_back(parse_cell(cur));
            }

            cur = m_lexer.next();
            return std::move(list);
        }

        // Just in case we come across a wayward ')'
        // the function above consumes any matched parens
        else if (cur == Token::Type::RPar) {
            std::ostringstream msg{};
            msg << "Unexpected ')' near "sv
                << m_lexer.row() << '-' << m_lexer.col();

            throw std::runtime_error{msg.str()};
        }

        // Do the conversion here to take advantage
        // of row and column information
        else if (cur == Token::Type::Num) {
            auto value = 0.0D;
            try {
                value = std::stod(cur.str());
            }

            catch (std::exception const & ex) {
                std::ostringstream msg{};
                msg << "Invalid number '"sv << cur.strview()
                    << "' near "sv << m_lexer.row() << '-'
                    << m_lexer.col();

                throw std::runtime_error{msg.str()};
            }

            auto atom = Number{value};
            cur = m_lexer.next();
            return std::move(atom);
        }

        else if (cur == Token::Type::Bool) {
            auto value = CIStringView{
                cur.strview().data(), cur.strview().size()
            };
            auto atom = Bool{value == "#t"_cisv};
            cur = m_lexer.next();
            
            return std::move(atom);
        }

        else if (cur == Token::Type::Id) {
            auto atom = Symbol{cur.strview()};
            cur = m_lexer.next();

            return std::move(atom);
        }

        else {
            std::ostringstream msg{};
            msg << "Unexpected token '"sv << cur.type() << "' near "sv
                << m_lexer.row() << '-' << m_lexer.col();
            throw std::runtime_error{msg.str()};
        }
    }
}
