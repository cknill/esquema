#include "parser.hh"
#include "gtest/gtest.h"
#include <string>
#include <tuple>
#include <vector>

namespace {
    using namespace std::literals::string_view_literals;
    using namespace std::literals::string_literals;
    using namespace esquema;
}

TEST(ParserTest, DefaultCellIsNil) {
    Cell cell{};
    ASSERT_TRUE(cell.is_nil())
        << "A default constructed Cell is Nil"sv;
}

TEST(ParserTest, BoolCellConstruction) {
    auto true_value = Bool{true};
    ASSERT_TRUE(true_value.value())
        << "Bool(bool) constructor failed to set m_value"sv;

    auto false_value = Bool{false};
    ASSERT_FALSE(false_value.value())
        << "Bool(bool) constructor failed to set m_value"sv;

    auto booleans = std::vector{ 
        std::pair{"#t"s, true}, std::pair{"#f"s, false},
        std::pair{"#F"s, false}, std::pair{"#T"s, true}
    };

    for (auto [str, val] : booleans) {
        Bool boolean{str};
        ASSERT_EQ(boolean.value(), val)
            << "Bool(str) constructed '"sv << boolean.value()
            << "' from '"sv << str << "'"sv;
    }
}

TEST(ParserTest, CellIsAtomTest) {
    auto cells = std::vector<Cell>{
        Number{1.24}, Symbol{"pi"s}, Bool{"#t"s} 
    };

    for (auto const & cell : cells) {
        ASSERT_TRUE(cell.is_atom())
            << cell << " is supposed to be an atom";
    }

    cells = std::vector<Cell>{
        Nil{}, List{Symbol{"foo"s}, Number{1}, Bool{false}}
    };

    for (auto const & cell : cells) {
        ASSERT_FALSE(cell.is_atom())
            << cell << " is not supposed to be an atom";
    }
}

TEST(ParserTest, ParseEmptyStrTest) {
    Parser parser{};
    auto src = ""s;
    auto res = parser.parse(src);
    ASSERT_TRUE(res.is_nil())
        << "Parsing an empty string must return Nil"sv;
}

TEST(ParserTest, ParseNumberTest) {
    Parser parser{};
    auto src = "1.234"s;
    auto res = parser.parse(src);
    ASSERT_TRUE(res.is_number())
        << "Parser failed to parse a simple number"sv;
}

TEST(ParserTest, ParseSymbolTest) {
    Parser parser{};
    auto src = "pi"s;
    auto res = parser.parse(src);
    ASSERT_TRUE(res.is_symbol())
        << "Parser failed to parse a simple symbol"sv;
}

TEST(ParserTest, ParseBoolTest) {
    Parser parser{};
    auto src = "#T"s;
    auto res = parser.parse(src);
    ASSERT_TRUE(res.is_bool())
        << "Parser failed to parse a simple boolean"sv;
}

TEST(ParserTest, ParseListTest) {
    Parser parser{};
    auto src = "(define pi 3.14)";
    auto res = parser.parse(src);
    ASSERT_TRUE(res.is_list())
        << "Parser failed to parse a list"sv;

    ASSERT_EQ(std::get<List>(res).size(), 3) 
        << "Parser failed to parse the list's elements"sv;
}

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
