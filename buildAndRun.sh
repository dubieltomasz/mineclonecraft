#!/bin/zsh

g++ -std=c++26 *.cpp -o tmp.o $(pkg-config --cflags --libs sdl3) && ./tmp.o && rm tmp.o
