# package all this up so that testing
# is easier
target_sources(
    esquema_lib
PUBLIC
    ci_string.hh ci_string.cc
    interp.hh interp.cc
    lexer.hh lexer.cc
    native_proc.hh native_proc.cc
    parser.hh parser.cc
)

# Look in this directory for headers
target_include_directories(
    esquema_lib
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# C++20 because we can
target_compile_features(
    esquema_lib
PUBLIC
    cxx_std_20
)

# The REPL proper
target_sources(
    esquema
PRIVATE
    main.cc
)

# Link in libnoise too
target_link_libraries(
    esquema
PRIVATE
     esquema_lib linenoise 
)
