#!/bin/bash

make clean && make
echo "===== START ./raymarcher scene0 shape0 shape1 shape2 shape3 shape4"
./raymarcher scene0 shape0 shape1 shape2 shape3 shape4
echo "==== END ./raymarcher scene0 shape0 shape1 shape2 shape3 shape4"
