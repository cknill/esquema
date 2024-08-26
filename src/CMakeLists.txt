target_sources(
    esquema_lib
PUBLIC
    interp.hh interp.cc
    lexer.hh lexer.cc
    native_proc.hh native_proc.cc
    parser.hh parser.cc
)

target_include_directories(
    esquema_lib
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_features(
    esquema_lib
PUBLIC
    cxx_std_20
)

target_sources(
    esquema
PRIVATE
    main.cc
)

target_link_libraries(
    esquema
PRIVATE
     esquema_lib linenoise 
)
