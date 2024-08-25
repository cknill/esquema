target_sources(
    esquema
PRIVATE
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
