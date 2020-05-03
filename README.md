# Ray Marcher
Implementation of a fast ray marcher.


# How to compile and run
### Compile
First compile the project by running these commands from the root directory:
```
cd build
make
```

A file called `raymarcher` will be created.

### Run
In the build folder run `./raymarcher`.
After the execution the output will be placed into the `output` directory.

### Compile and Run
If you want to simply compile and run all the predefined scenes, then run the following:

On UNIX:
```console
team049$ cd build
team049/build$ bash run_raymarcher.sh 
rm -f obj/*.o *~ core /*~ 
gcc -c -o obj/raymarcher.o ../src/raymarcher.c -O3 -I../include -g
gcc -c -o obj/lodepng.o ../src/lodepng.c -O3 -I../include -g
gcc -c -o obj/vec3.o ../src/vec3.c -O3 -I../include -g
gcc -c -o obj/mat4.o ../src/mat4.c -O3 -I../include -g
gcc -c -o obj/camera.o ../src/camera.c -O3 -I../include -g
gcc -c -o obj/utility.o ../src/utility.c -O3 -I../include -g
gcc -c -o obj/light.o ../src/light.c -O3 -I../include -g
gcc -c -o obj/scene_loader.o ../src/scene_loader.c -O3 -I../include -g
gcc -c -o obj/material.o ../src/material.c -O3 -I../include -g
gcc -c -o obj/benchmark.o ../src/benchmark.c -O3 -I../include -g
gcc -o raymarcher obj/raymarcher.o obj/lodepng.o obj/vec3.o obj/mat4.o obj/camera.o obj/utility.o obj/light.o obj/geometry/scene.o obj/geometry/plane.o obj/geometry/sphere.o obj/geometry/box.o obj/geometry/cone.o obj/geometry/octahedron.o obj/geometry/torus.o obj/geometry/transform.o obj/geometry/mandelbulb.o obj/scene_loader.o obj/material.o obj/benchmark.o -O3 -I../include -g -lm
===== START ./raymarcher scene0 shape0 shape1 shape2 shape3 shape4
Add scene scene0 into index 0.
Add scene shape0 into index 1.
Add scene shape1 into index 2.
Add scene shape2 into index 3.
Add scene shape3 into index 4.
Add scene shape4 into index 5.
RENDERING...  0.99
Image rendered and saved in path folder ../output/render_out/scene0.png 
RENDERING...  0.99
Image rendered and saved in path folder ../output/render_out/shape0.png 
RENDERING...  0.99
Image rendered and saved in path folder ../output/render_out/shape1.png 
RENDERING...  0.99
Image rendered and saved in path folder ../output/render_out/shape2.png 
RENDERING...  0.99
Image rendered and saved in path folder ../output/render_out/shape3.png 
RENDERING...  0.99
Image rendered and saved in path folder ../output/render_out/shape4.png 
==== END ./raymarcher scene0 shape0 shape1 shape2 shape3 shape4
```

### Scenes
A Scene contains geometric objects in a 3D space that the raymarcher uses to render an image. 
The currently available scenes are in the team049/scenes/ folder.

If an custom scene wants to be rendered then create a JSON file with the predefined structure.
### Test
On the command line on the root of the team049 directory execute the following:

```console
$ cd tests
$ make //this compiles the files that are required for the test
$ ./test 
```
An example output might be the following: 

```console
team049/tests$ ./test

* Suite main_suite:
............
12 tests - 12 passed, 0 failed, 0 skipped (60 ticks, 0.000 sec)

Total: 12 tests (133 ticks, 0.000 sec), 30 assertions
Pass: 12, fail: 0, skip: 0.
```

You may need to change the permission to run_tests.sh script:
Here an example output
```console
team049/tests$ ./run_tests.sh
bash: ./run_tests.sh: Permission denied
team049/tests$ ls -l run_tests.sh
-rw-r--r-- 1 dave dave 64 avril 25 11:02 run_tests.sh
team049/tests$ chmod 755 run_tests.sh
team049/tests$ ls -l run_tests.sh
-rwxr-xr-x 1 dave dave 64 avril 25 11:02 run_tests.sh
```
then you can simply run the run_tests.sh bash script.

Another way of running the run_tests.sh script is:
```console
team049/tests$ bash run_tests.sh
```
Another windows compatible version: run_tests_win.sh is available too.


# Add test cases

In folder tests:
add the test file you want to create, i.e. see test_example.h
In test_example.h you find also an example on how to write a test.

On the file tests/tests.c:
Create a new SUITE for you test cases or add them to the main suite.
i.e.
```c
SUITE(<SUITE NAME>){   
    /*
    *   Here you can add other test cases
    *   i.e. RUN_TEST(<TEST NAME GOES HERE>);
    */
}
```

In the file tests/makefile:
add the newly created header file containing the tests:
```makefile
TDEPS = vec3_test.h test_example.h <YOUR HEADER FILE>
```
If you want to modify the compilation flags modify the tests/makefile:
```makefile
TFLAGS =  -I$(IDIR) -lm -g -Wall -Werror -pedantic
```


# Organizational
In order to have the best team work we also use additional tools:

- **Working directory**: Google Drive.
- **Task management**: Trello.

If access to these platform is required contact us.



## Authors
**Nihat Isik - (github: devnio)**

**Qais El Okaili - (github: Xemno)**

**Alexandre Cavaleri - (github: cavEpfl)** 

**David Graf  - (github: fxd24)**

Note that some commits were done on computers with different global git configurations, which ultimately resulted in commits authored by our github (and not gitlab) user names. 
For the sake of clarity we attach the github name on the right of each team member name.