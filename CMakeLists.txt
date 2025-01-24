cmake_minimum_required(VERSION 3.30.0 FATAL_ERROR)

enable_testing()

# C++ Language-specific variables, setting build env constraints
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/elibster/the_deck")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    add_compile_options(-stdlib=libc++ -fexperimental-library)
  endif()
endif()

project(The_Deck
    VERSION 1.0.0
    DESCRIPTION "The deck of all things learning"
    LANGUAGES CXX C
    HOMEPAGE_URL CMAKE_PROJECT_HOMEPAGE_URL)

set(INSTALL_GTEST OFF)
add_subdirectory(decky)
add_subdirectory(tests)
add_subdirectory(examples)

if (UNIX OR APPLE)
  include(GNUInstallDirs)
endif()

install(TARGETS the_deck LIBRARY)
install(FILES decky/the_deck.h DESTINATION include)
