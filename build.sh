#!/bin/bash

time bash -c "g++ -std=c++2a -O3 -I./src -o out/main src/main.cpp && out/main"
