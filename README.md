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
I’m assuming you’re building from a PowerShell developer prompt.

```powershell
PS> Remove-Item -Recurse -Force build
PS> Remove-Item -Recurse -Force artifacts
PS> New-Item -Name build -ItemType Directory
PS> New-Item -Name artifacts -ItemType Directory
PS> New-Item -Name artifacts\dev -ItemType Directory
PS> New-Item -Name artifacts\gtest -ItemType Directory
PS> Set-Location -Path build
PS> cmake -S .. -B . 
PS> msbuild the_deck.sln -p:Configuration=Release
PS> Copy-Item decky\Release\* -Destination ..\artifacts\dev
PS> Copy-Item ..\decky\the_deck.h -Destination ..\artifacts\dev
PS> Copy-Item bin\Release\gtest.dll -Destination ..\artifacts\gtest
PS> Copy-Item bin\Release\gtest_main.dll -Destination ..\artifacts\gtest
PS> Copy-Item tests\Release\decky_gtest.exe -Destination ..\artifacts\gtest
PS> Set-Location -Path ..
PS> Remove-Item -Recurse -Force build
```

This is provided for you as `build.ps1`.

The useful build artifacts are `the_deck.lib`, `the_deck.dll`, and 
`the_deck.exp`. They can all be found under `artifacts\dev`.

## Testing
After running the build script, `artifacts\gtest\decky_gtest.exe`.

## Developing
Ensure that `the_deck.h` is somewhere on your include path. You can then
do something like:

```powershell
PS> cl.exe /EHsc /O1 /std:c++latest /Iartifacts\dev \
    examples\encrypt.cpp /Fesolitaire_encrypt.exe /link \
    artifacts\dev\the_deck.lib
PS> Copy-Item artifacts\dev\the_deck.dll -Destination .
PS> .\solitaire_encrypt.exe
```

Run `solitaire_encrypt.exe`. Enter some text followed by Ctrl-D to exit.


# MacOS and UNIX

## Building
```bash
$ rm -rf build
$ mkdir build
$ cd build
$ cmake -S.. -B. -GNinja -DCMAKE_BUILD_TYPE=Release
$ ninja
$ sudo ninja install
$ cd ..
```

There’s an included shell script (`build.sh`) to do it all for you.

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
