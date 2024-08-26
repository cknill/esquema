add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
add_executable(lexer_test lexer_test.cc)
target_include_directories(
    lexer_test
PRIVATE
    ${ESQUEMA_SOURCE_DIR}
)

target_link_libraries(
    lexer_test
PRIVATE
    esquema_lib GTest::GTest
)

add_test(gtest_lexer_test lexer_test)

add_executable(parser_test parser_test.cc)
target_include_directories(
    parser_test
PRIVATE
    ${ESQUEMA_SOURCE_DIR}
)

target_link_libraries(
    parser_test
PRIVATE
    esquema_lib GTest::GTest
)

add_test(gtest_parser_test parser_test)

add_executable(interpreter_test interp_test.cc)
target_include_directories(
    interpreter_test
PRIVATE
    ${ESQUEMA_SOURCE_DIR}
)

target_link_libraries(
    interpreter_test
PRIVATE
    esquema_lib GTest::GTest
)

add_test(gtest_interpreter_test interpreter_test)
