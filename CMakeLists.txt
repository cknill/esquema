cmake_minimum_required(VERSION 3.18)

project(Esquema LANGUAGES CXX VERSION 0.1.0)

# Let the user turn off testing if they want
option(
    ESQUEMA_BUILD_TESTS "Build the unit tests" ON
)

if (NOT ESQUEMA_BUILD_TESTS)
    set(BUILD_TESTING Off)
endif()

# This will make it easier to build the tests later
set(ESQUEMA_SRC_DIR ${CMAKE_PROJECT_SOURCE_DIR}/src)

# Grab linenoise from GIT and make a happy target to it 
include(FetchContent)
FetchContent_Declare(
    libnoise
    GIT_REPOSITORY https://github.com/yhirose/cpp-linenoise
)
FetchContent_MakeAvailable(libnoise)

add_executable(esquema "")

# I tried making an OBJECT library but got lots
# of linker errors. I've done it before with
# great success. I don't know if it's a Ninja
# thing or if something else changed since I 
# did it last
add_library(esquema_lib STATIC "")

add_subdirectory(src)

# Only if we want the tests grab
# googletest from GIT and enable ctest
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

# Installation goodness, not going to install esquema_lib
# because it was never intended to be a library
include(GNUInstallDirs)
install(TARGETS esquema DESTINATION bin)

