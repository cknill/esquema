#include "interp.hh"
#include "gtest/gtest.h"
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>
#include <iterator>

namespace {
    using namespace std::literals::string_view_literals;
    using namespace std::literals::string_literals;
    using namespace esquema;
}

TEST(InterpreterTest, DefaultEnvironmentConstructorTest) {
    Environment env{};
    auto num_elems = std::distance(env.begin(), env.end());
    ASSERT_EQ(num_elems, 0)
        << "Default constructed Environment must be empty, got "sv
        << num_elems << " elements"sv;
}

TEST(InterpreterTest, EnvironmentInsertFindTest) {
    Environment env{};
    auto sym = Symbol{"pi"s};
    env.insert(sym, Number{3.14});
    auto num_elems = std::distance(env.begin(), env.end());
    ASSERT_EQ(num_elems, 1)
        << "Environment failed to insert an element"sv;

    auto it = env.find(sym);
    ASSERT_NE(it, env.end())
        << "Environment failed to find inserted element"sv;

    ASSERT_TRUE(it->second.is_number())
        << "Environment retrieved the wrong element"sv;

    ASSERT_EQ(std::get<Number>(it->second).value(), 3.14)
        << "Environment retrieved the wrong element"sv;

    auto other_sym = Symbol{"foo"s};
    it = env.find(other_sym);
    ASSERT_EQ(it, env.end())
        << "Environment retrieved a non-existent symbol"sv;
}

TEST(InterpreterTest, ParentEnvironmentFindTest) {
    auto global = Environment{};
    auto sym = Symbol{"pi"s};
    global.insert(sym, Number{3.14});
    auto local = Environment{&global};
    auto it = local.find(sym);
    ASSERT_NE(it, local.end())
        << "Environment find failed to search global scope"sv;

    ASSERT_TRUE(it->second.is_number())
        << "Environment retrieved the wrong symbol"sv;

    ASSERT_EQ(std::get<Number>(it->second).value(), 3.14)
        << "Environment retrieved the wrong symbol"sv;

    auto other_sym = Symbol{"foo"s};
    it = local.find(other_sym);
    ASSERT_EQ(it, local.end())
        << "Environment found a non-existent symbol"sv;
}

TEST(InterpreterTest, GlobalEnvironmentTest) {
    auto global_keys = std::vector{
       "+"s, "-"s, "*"s, "/"s, "<"s, "<="s, 
       ">"s, ">="s, "eqv?"s, "not"s, "pi"s, 
       "e"s  
    };

    std::sort(global_keys.begin(), global_keys.end());
    auto global_env = Environment::make_global();
    std::vector<std::string> test_keys{};
    test_keys.reserve(std::distance(global_env.begin(), global_env.end()));
    for (auto const & [k, v] : global_env) {
        test_keys.push_back(k);
    }

    std::sort(test_keys.begin(), test_keys.end());
    ASSERT_EQ(global_keys, test_keys)
        << "Environment is missing the default keys"sv;
}

TEST(InterpreterTest, EmptyProgramTest) {
    auto src = ""s;
    Interpreter interp{};
    auto res = interp.eval(src);
    ASSERT_TRUE(res.is_nil())
        << "Empty program must evaluate to Nil"sv;
}

TEST(InterpreterTest, DefineTest) {
    auto src = "(define x 42)"s;
    Interpreter interp{};
    auto res = interp.eval(src);
    ASSERT_TRUE(res.is_nil())
        << "The define operation must return Nil"sv;

    src = "x"s;
    res = interp.eval(src);
    ASSERT_TRUE(res.is_number())
        << "Interpreter failed to properly bind 42 to x"sv;

    auto x = std::get<Number>(res).value();
    ASSERT_EQ(x, 42)
        << "Interpreter failed to properly bind 42 to x"sv;
}

TEST(InterpeterTest, AdditionTest) {
    auto ground_truth = std::vector{
        std::pair{"(+ 2 2)"s, 4}, std::pair{"(+ 1 2 3)"s, 6},
        std::pair{"(+ (+ 2 0) (+ 0 2))"s, 4}
    };

    Interpreter interp{};
    for (auto const & [src, truth] : ground_truth) {
        auto res = interp.eval(src);
        ASSERT_TRUE(res.is_number())
            << "Interpreter failed to reduce to a number"sv;

        ASSERT_EQ(std::get<Number>(res).value(), truth)
            << "Addition produced the wrong result: expected "sv
            << truth << " got "sv << std::get<Number>(res).value();
    }
}

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
