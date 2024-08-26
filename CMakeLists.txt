cmake_minimum_required(VERSION 3.18)
project(Esquema LANGUAGES CXX VERSION 0.1.0)
set(ESQUEMA_BUILD_TESTS On)
option(
    ESQUEMA_BUILD_TESTS "Build the unit tests"
)

include(FetchContent)
FetchContent_Declare(
    libnoise
    GIT_REPOSITORY https://github.com/yhirose/cpp-linenoise
)

FetchContent_MakeAvailable(libnoise)

add_executable(esquema "")
add_subdirectory(src)
add_subdirectory(tests)

include(GNUInstallDirs)

