#!/bin/zsh

g++ -std=c++26 *.cpp -o tmp $(pkg-config --cflags --libs sdl3) && ./tmp && rm tmp
