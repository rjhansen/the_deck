# Rob’s branch of `the_deck`

## Tested compilers

* Microsoft Visual Studio 2022 on x86_64 Windows 11
* Clang 19.1.5 on x86_64 Linux
* Intel C++ 2025.0.4 on x86_64 Linux
* GNU C++ 14.2.1 on x86_64 Linux
* Clang 16 on Apple Silicon

# Why this branch?

Over two days of work we pushed `the_deck` from nothing to a minimum viable
product. We were unable to spend the time to give it the polish and attention
it deserved. So, one Friday night, I decided to do that.

The overall changes are minor: for instance, since `Suit` and `Rank` have no
meaning outside of a `Card`, those `enums` were moved into the `Card` class.

Some old-style STL algorithms were replaced by their modern Ranges
equivalents.