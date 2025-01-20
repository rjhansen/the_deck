# Rob’s branch of `the_deck`

Over two days of work we pushed `the_deck` from nothing to a minimum viable
product. We were unable to spend the time to give it the polish and attention
it deserved. So, I decided to do that.

The overall changes are minor: for instance, since `Suit` and `Rank` have no
meaning outside of a `Card`, those `enums` were moved into the `Card` class.

Some old-style STL algorithms were replaced by their modern Ranges and/or 
Views equivalents.

# Tested compilers

* Microsoft Visual Studio 2022 on x86_64 Windows 11
* Clang 19.1.5 on x86_64 Linux
* Intel C++ 2025.0.4 on x86_64 Linux
* GNU C++ 14.2.1 on x86_64 Linux
* Clang 16 on Apple Silicon

# Building

## UNIX and MacOS

It’s as simple as:

```
$ rm -rf build
$ mkdir build
$ cd build
$ cmake -S.. -B. -GNinja -DCMAKE_BUILD_TYPE=Release
$ ninja
$ sudo ninja install
$ cd ..
$ rm -rf build
```

In fact, it’s so simple there’s an included shell script to do it
for you: `build.sh`!

## MacOS-specific instructions
I haven’t yet figured out how to do `@rpath` substitution, so for 
now please add the line 

```
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/usr/local/lib
```

… to your `$HOME/.zprofile` file.

# Using `the_deck`

Once you have it installed, using it in your own code is
straightforward.

```
$ clang++ -std=c++23 -stdlib=libc++ -fexperimental-library \
  -O2 examples/encrypt.cpp -o solitaire_encrypt -lthe_deck
```

Note that since `the_deck` uses features from the latest C++23
standard, each compiler will require different incantations in
order to work. These instructions work with Clang: other
compilers, you’re on your own.
