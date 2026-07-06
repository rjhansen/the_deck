# Rob’s fork of `the_deck`

Over the Christmas holidays I taught a weeklong course in modern C++ design.
The major project was an implementation of Bruce Schneier’s Solitaire
encryption algorithm, as made famous in Neal Stephenson’s
[_Cryptonomicon_](https://www.amazon.com/Cryptonomicon-Neal-Stephenson/dp/0060512806).
It was a good project that forced students to explore some of the new
facilities provided by the C++23 standard.

At the end of this we had a minimum viable product for a Solitaire library,
and I decided it was very close to being an excellent starting point for
people looking for a model for modern C++ code. So, with the permission of the
student who wrote the core of this, I’ve completed it.

# How is it licensed?

Apache 2.0. Share and enjoy, my friends.

# What does it do?

It implements an encryption algorithm, but does so using a crossplatform
build environment that works with pretty much every C++ compiler out there
and shows:

* modern C++ design
* CMake usage
* building shared libraries
* how to use this as a structure for your own code

# Tested compilers

| Vendor            | Compiler | Version | OS            | Processor |
|:------------------|:--------:|:-------:|:-------------:|----------:|
| Microsoft         | MSVC 2022| 2022    | Windows 11    | x86_64    |
| Mingw             | clang++  | 19      | Windows 11    | x86_64    |
| Mingw             | g++      | 14      | Windows 11    | x86_64    |
| Intel             | icpx     | 2025    | Fedora 41     | x86_64    |
| GNU               | g++      | 14      | Fedora 41     | x86_64    |
| LLVM              | clang++  | 2025    | Fedora 41     | x86_64    |
| Apple             | clang++  | 21      | macOS Sequoia | Apple Silicon |
