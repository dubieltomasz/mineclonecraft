#!/bin/zsh

g++ -std=c++26 *.cpp -o build.o $(pkg-config --cflags --libs sdl3)
