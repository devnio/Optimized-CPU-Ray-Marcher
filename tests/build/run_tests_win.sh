#!/bin/bash

cd ../build && mingw32-make clean && mingw32-make
cd ../tests && mingw32-make clean && mingw32-make
./test.exe