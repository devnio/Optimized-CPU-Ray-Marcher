#!/bin/bash

cd build && make clean && make
echo "===== START ./raymarcher"
./raymarcher
echo "==== END ./raymarcher"
echo "===== START ./raymarcher scene0"
./raymarcher scene0
echo "==== END ./raymarcher shape0"
echo "===== START ./raymarcher shape0"
./raymarcher shape0
echo "==== END ./raymarcher shape0"
echo "===== START ./raymarcher shape1"
./raymarcher shape1
echo "==== END ./raymarcher shape1"
echo "===== START ./raymarcher shape2"
./raymarcher shape2
echo "==== END ./raymarcher shape2"
echo "===== START ./raymarcher shape3"
./raymarcher shape3
echo "==== END ./raymarcher shape3"
echo "===== START ./raymarcher shape4"
./raymarcher shape4
echo "==== END ./raymarcher shape4"