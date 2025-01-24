# Rob’s fork of `the_deck`

Over the Christmas holidays I taught a weeklong course in modern C++ design.
The major project was an implementation of Bruce Schneier’s Solitaire
encryption algorithm, as made famous in Neal Stephenson’s 
[_Cryptonomicon_](https://www.amazon.com/Cryptonomicon-Neal-Stephenson/dp/0060512806).
It was a good project that forced students to explore some of the new
facilities provided by the C++20 standard.

At the end of this we had a minimum viable product for a Solitaire library,
and I decided it was very close to being an excellent starting point for
people looking for a model for modern C++ code. So, with the permission of the
student who wrote the core of this, I’ve completed it.

# What does it do?
It implements an encryption algorithm, but does so using a crossplatform
build environment that works with pretty much every C++ compiler out there
and shows:

* modern C++ design
* CMake usage
* building shared and static libraries
* how to use this as a structure for your own code

# Tested compilers

* Microsoft Visual Studio 2022 on x86_64 Windows 11
* Clang 19.1.5 on x86_64 Linux
* Intel C++ 2025.0.4 on x86_64 Linux
* GNU C++ 14.2.1 on x86_64 Linux
* Clang 16 on Apple Silicon

# Windows
## Building
Run `build.ps1`. This will create an output directory, `the_deck-win64-1.0.0`,
populated with build artifacts and a test suite. It will also build an archive
file, `the_deck-win64-1.0.0.zip`, for your convenience.

## Testing
After running the build script, executing
`the_deck-win64-1.0.0\test_suite\decky_gtest.exe` will run the self-tests.

## Developing
Ensure that `the_deck.h` is somewhere on your include path. You can then
do something like:

```powershell
PS> cl.exe /EHsc /O1 /std:c++latest /Iartifacts\dev \
    examples\encrypt.cpp /Fesolitaire_encrypt.exe /link \
    artifacts\dev\the_deck.lib
PS> Copy-Item the_deck-win64-1.0.0\dev\the_deck.dll -Destination .
PS> .\solitaire_encrypt.exe
```

Run `solitaire_encrypt.exe`. Enter some text followed by Ctrl-D to exit.


# MacOS and UNIX

## Building
There’s an included shell script (`build.sh`) to do it all for you.
You can also use `build.ps1` if you really want to get funky.

### macOS-specific instructions
I haven’t yet figured out how to do `@rpath` substitution, so for 
now please add the line 

```bash
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/usr/local/lib
```

… to your `$HOME/.zprofile` file.

### Linux-specific instructions
Like with macOS (see above), please either do a similar `export` (but
this time, of `LD_LIBRARY_PATH`), or else add `/usr/local/lib64` and/or
`/usr/local/lib` to your `/etc/ld.so.conf` file and run `sudo ldconfig`.

## Testing

From within the build tree, `ctest` should do well.

## Developing

```bash
$ clang++ -std=c++23 -stdlib=libc++ -fexperimental-library \
  -O2 examples/encrypt.cpp -o solitaire_encrypt -lthe_deck
$ ./solitaire_encrypt
```

Alternately,
```bash
$ g++ -std=c++23 -O2 examples/encrypt.cpp -o solitaire_encrypt \
  -lthe_deck
$ ./solitaire_encrypt
```

Run `solitaire_encrypt`. Enter some text followed by Ctrl-D to exit.
