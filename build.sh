#!/bin/bash
mkdir -p build
cd build
cmake ..
make
./COMP345-A1-Part1
cd ..