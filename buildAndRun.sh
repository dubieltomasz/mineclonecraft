#!/bin/zsh

g++ -std=c++26 main.cpp -o tmp $(pkg-config --cflags --libs sdl3) && ./tmp && rm tmp
