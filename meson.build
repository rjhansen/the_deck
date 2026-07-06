project('the_deck', 'cpp', default_options: ['cpp_std=c++23'])
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
deck_tests = ['tests/decky_gtest.cpp']
deck_includes = include_directories('decky')
deck_sources = ['decky/card.cpp', 'decky/deck.cpp', 'decky/solitaire.cpp']
deck_lib = shared_library(
    'the_deck',
    sources: [deck_sources],
    include_directories: [deck_includes],
    install: true,
)
install_headers('decky/the_deck.h')
deck_test = executable(
    'unit_tests',
    sources: [deck_tests],
    include_directories: [deck_includes],
    dependencies: [gmock_dep, gtest_dep],
    link_with: [deck_lib],
    install: false,
)
executable(
    'sol-encrypt',
    sources: ['examples/encrypt.cpp'],
    include_directories: [deck_includes],
    link_with: [deck_lib],
    install: true,
)
executable(
    'sol-decrypt',
    sources: ['examples/decrypt.cpp'],
    include_directories: [deck_includes],
    link_with: [deck_lib],
    install: true,
)
test('unit_tests', deck_test)
