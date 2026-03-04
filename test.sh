#!/bin/sh

mkdir ./build
cd ./build
cmake ..
cmake --build .
./MineCloneCraft
cd ..
rm -rf ./build
