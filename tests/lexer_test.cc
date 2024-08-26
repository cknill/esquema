#include "lexer.hh"
#include "gtest/gtest.h"
#include <string>
#include <vector>

namespace {
    using namespace std::literals::string_view_literals;
    using namespace std::literals::string_literals;
    using namespace esquema;
}

TEST(LexerTest, DefaultTokenIsError) {
    Token token{};
    ASSERT_EQ(token.type(), Token::Type::Err)
        << "Default token's type must be Err"sv;
}

TEST(LexerTest, DefaultTokenIsEmptyStr) {
    Token token{};
    ASSERT_EQ(token.strview(), ""sv)
        << "Default token must have the empty string"sv;
}

TEST(LexerTest, TokenConstructorTest) {
    Token token{Token::Type::LPar};
    ASSERT_EQ(token.type(), Token::Type::LPar)
        << "Token(Type) must initialize m_type with Type"sv;

    ASSERT_EQ(token.strview(), ""sv)
        << "Token(Type) must initialize m_txt with the empty string"sv;

    Token token_two{Token::Type::Id, "foo"sv};
    ASSERT_EQ(token_two.type(), Token::Type::Id)
        << "Token(Type, std::string_view) must initialize m_type with Type"sv;

    ASSERT_EQ(token_two.strview(), "foo"sv)
        << "Token(Type, std::string_view) must initialize m_txt with std::string_view"sv;
}

TEST(LexerTest, EmptyLexerTest) {
    Lexer lexer{};
    auto token = lexer.next();
    ASSERT_EQ(token.type(), Token::Type::Eof)
        << "Empty lexer must return EOF"sv;

    ASSERT_EQ(lexer.row(), -1)
        << "Empty lexer row() must return -1"sv;

    ASSERT_EQ(lexer.col(), -1)
        << "Empty lexer col() must return -1"sv;

    lexer = Lexer{""sv};
    token = lexer.next();
    ASSERT_EQ(token.type(), Token::Type::Eof)
        << "Lexing an empty string must return EOF"sv;

    ASSERT_EQ(lexer.row(), -1)
        << "Lexer row() w/ empty string must return -1"sv;

    ASSERT_EQ(lexer.col(), -1) 
        << "Lexer col() w/ empty string must return -1"sv;
}

TEST(LexerTest, WSToEOF) {
    auto txt = " \t\r\n"s;
    Lexer lexer{txt};
    auto token = lexer.next();
    ASSERT_EQ(token.type(), Token::Type::Eof)
        << "Lexer must ignore all WS"sv;

    // There was a newline in the whitespace
    // so column goes back to 1
    ASSERT_EQ(lexer.col(), 1)
        << "Lexer must track WS"sv;

    ASSERT_EQ(lexer.row(), 2)
        << "Lexer must track WS"sv;
}

TEST(LexerTest, LexNumberTest) {
    auto numbers = std::vector{
        "0123"s, "+0123"s, "-0123"s,
        "0.5678"s, "+0.5678"s, "-0.5678"s,
        ".90123"s, "+.90123"s, "-.90123"s
    };

    Lexer lexer{};
    Token token{};
    for (auto const & number : numbers) {
        lexer = Lexer{number};
        token = lexer.next();

        ASSERT_EQ(token.type(), Token::Type::Num)
            << "Lexer failed to recognize '"sv 
            << number << "' as a number"sv;

        ASSERT_EQ(token.str(), number)
            << "Lexer extracted '"sv << token.str()
            << "' instead of '"sv << number << "'"sv;
    }
}

TEST(LexerTest, LexIdTests) {
    auto ids = std::vector{
        "foo"s, "bar"s, "baz"s,
        "Foo-bar"s, "foo-Bar-baz"s,
        "FooBarBAZ123"s, 
        "+"s, "-"s, "<=-"s, "->"s
    };

    Lexer lexer{};
    Token token{};
    for (auto const & id : ids) {
        lexer = Lexer{id};
        token = lexer.next();
        ASSERT_EQ(token.type(), Token::Type::Id)
            << "Lexer failed to recognize '"sv << id
            << "' as an Id"sv;

        ASSERT_EQ(token.str(), id)
            << "Lexer extracted '"sv << token.str() << "' instead of '"sv
            << id << "'"sv;
    }
}

TEST(LexerTest, LexBoolTests) {
    auto booleans = std::vector{
        "#t"s, "#T"s, "#f"s, "#F"s
    };

    Lexer lexer{};
    Token token{};
    for (auto const & boolean : booleans) {
        lexer = Lexer{boolean};
        token = lexer.next();
        ASSERT_EQ(token.type(), Token::Type::Bool)
            << "Lexer failed to recognize '"sv << boolean << "' as a boolean"sv;

        ASSERT_EQ(token.str(), boolean)
            << "Lexer extracted '" << token.str() << "' instead of '"sv
            << boolean << "'"sv;
    }
}

TEST(LexerTest, LexParensTest) {
    auto txt = "  (  "s;
    Lexer lexer{txt};
    auto token = lexer.next();
    ASSERT_EQ(token.type(), Token::Type::LPar)
        << "Lexer failed to recognize '"sv << txt << "' as a left paren"sv;

    txt = "  )  "s;
    lexer = Lexer{txt};
    token = lexer.next();
    ASSERT_EQ(token.type(), Token::Type::RPar)
        << "Lexer failed to recognize '" << txt << "' as a right paren"sv;
}

TEST(LexerTest, LexCommentTest) {
    auto txt = ";This is just like ws\n1234"s;
    Lexer lexer{txt};
    auto token = lexer.next();
    ASSERT_EQ(token.type(), Token::Type::Num)
        << "Lexer failed to ignore the comment"sv;
}

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
