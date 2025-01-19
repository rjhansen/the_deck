cmake_minimum_required(VERSION 3.29.0 FATAL_ERROR)

# C++ Language-specific variables, setting build env constraints
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/elibster/the_deck")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    add_compile_options(-stdlib=libc++ -fexperimental-library)
  endif()
endif()

set(ENCRYPTOR_APP encryptor)
set(DECRYPTOR_APP decryptor)

project(The_Deck
    VERSION 1.0.0
    DESCRIPTION "The deck of all things learning"
    LANGUAGES CXX C
    HOMEPAGE_URL CMAKE_PROJECT_HOMEPAGE_URL)


add_subdirectory(decky)

target_include_directories(${ENCRYPTOR_APP} PUBLIC decky)
target_link_libraries(${ENCRYPTOR_APP} PUBLIC decky)
target_include_directories(${DECRYPTOR_APP} PUBLIC decky)
target_link_libraries(${DECRYPTOR_APP} PUBLIC decky)

# Set up the testing environment
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(decky_gtest tests/decky_gtest.cpp)
target_link_libraries(decky_gtest decky GTest::gtest_main)
target_include_directories(decky_gtest PUBLIC decky)
add_test(NAME decky_gtest COMMAND decky_gtest)