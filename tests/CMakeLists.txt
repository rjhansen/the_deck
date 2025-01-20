include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

Include(GoogleTest)
add_executable(decky_gtest decky_gtest.cpp)
target_include_directories(decky_gtest PUBLIC ../decky)
target_link_libraries(decky_gtest the_deck GTest::gtest_main)
gtest_add_tests(TARGET decky_gtest)