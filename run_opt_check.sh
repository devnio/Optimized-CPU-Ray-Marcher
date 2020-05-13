#!/bin/bash

# ==== RUN OPT CODE

cd build
bash run_raymarcher.sh
cd ../tests/build/
bash run_tests.sh