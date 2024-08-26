cmake_minimum_required(VERSION 3.18)

project(Esquema LANGUAGES CXX VERSION 0.1.0)

option(
    ESQUEMA_BUILD_TESTS "Build the unit tests" ON
)

if (NOT ESQUEMA_BUILD_TESTS)
    set(BUILD_TESTING Off)
endif()

set(ESQUEMA_SRC_DIR ${CMAKE_PROJECT_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(
    libnoise
    GIT_REPOSITORY https://github.com/yhirose/cpp-linenoise
)
FetchContent_MakeAvailable(libnoise)

add_executable(esquema "")
add_library(esquema_lib STATIC "")
add_subdirectory(src)
if (ESQUEMA_BUILD_TESTS)
    include(CTest)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.15.2
    )
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests)
endif()

include(GNUInstallDirs)

