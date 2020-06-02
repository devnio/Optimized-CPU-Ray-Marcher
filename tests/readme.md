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
