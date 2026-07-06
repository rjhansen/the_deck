#!/bin/sh

rm -rf build subprojects/googletest* subprojects/packagecache
meson setup build --buildtype=debugoptimized --prefix=$HOME
cd build
meson compile
meson test
meson install
cd ..
rm -rf build subprojects/googletest* subprojects/packagecache
