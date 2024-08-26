target_sources(
    esquema
PRIVATE
    interp.hh interp.cc
    lexer.hh lexer.cc
    native_proc.hh native_proc.cc
    parser.hh parser.cc
    main.cc
)

target_include_directories(
    esquema
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_features(
    esquema
PRIVATE
    cxx_std_20
)
