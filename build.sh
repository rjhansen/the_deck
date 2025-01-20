#!/bin/sh

rm -rf build
mkdir build
cd build
cmake -S.. -B. -GNinja -DCMAKE_BUILD_TYPE=Release
ninja
sudo ninja install
cd ..
rm -rf build
