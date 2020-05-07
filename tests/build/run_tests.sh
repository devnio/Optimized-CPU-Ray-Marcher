#!/bin/bash

cd ../build && make clean && make
cd ../tests && make clean && make
./test