#!/usr/bin/env bash
# Cross compile script to build windows binaries
# Depends on Mingw-w64

export CC_FLAGS="-Wall -std=c++11 -O3 -flto -pthread -mtune=generic -fno-exceptions -static"
export LD_FLAGS="-pthread -flto -static"
export CXX=x86_64-w64-mingw32-g++-posix
make clean
make -j4
mv shallowblue shallowblue_x86-64.exe

export CC_FLAGS="-Wall -std=c++11 -O3 -flto -pthread -mtune=generic -static -m32"
export LD_FLAGS="-pthread -flto -static -m32"
export CXX=i686-w64-mingw32-g++-posix
make clean
make -j4
mv shallowblue shallowblue_x86.exe

echo -e "\n==========="
echo "Windows builds created"
echo "SHA256 sums"
sha256sum shallowblue_x86.exe shallowblue_x86-64.exe
