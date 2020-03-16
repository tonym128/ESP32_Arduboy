#!/bin/bash

clear; clear; g++ -x c++ -g -std=c++14 -fmax-errors=5 -pedantic -Wall -Wextra -o game microtd.ino 2>&1 >/dev/null && ./game
