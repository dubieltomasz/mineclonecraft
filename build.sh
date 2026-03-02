#!/bin/zsh

clang++ -std=c++26 -target x86_64-w64-windows-gnu main.cpp -o build.exe $(pkg-config --cflags --libs sdl3)
