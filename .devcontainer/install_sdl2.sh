#!/usr/bin/env bash

cd /tmp
git clone https://github.com/libsdl-org/SDL.git --depth=1 -b SDL2
cd SDL
mkdir build
cd build
../configure
make
make install
rm -rf /tmp/SDL2