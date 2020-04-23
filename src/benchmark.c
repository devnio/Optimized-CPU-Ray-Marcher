#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "utility.h"
#include "benchmark/benchmark.h"
#include "benchmark/tsc_x86.h"
// #ifndef TSC_X86_H
//   #include "benchmark/tsc_x86.h"
// #endif

#define CYCLES_REQUIRED 1e7
#define REPETITIONS 1
#define FLOPS (4.*n)
#define EPS (1e-3)
#define NUM_RUNS 1
#define FREQUENCY 2.7e9
#define CALIBRATE
#define NR_OF_SAMPLES 30

#define START_W_RESOLUTION 10 // width resolution with which we want to start with
#define END_W_RESOLUTION 100 // width resolution we want to reach
#define RESOLUTION_STEPS 10
#define SCALE_RATIO 1.77777778f // scale ratio width to height of image

#define MAX_NR_OF_FUNCS 32
#define MAX_STRING_SIZE 40

render_func_prot functions[MAX_NR_OF_FUNCS]; // array of function pointers
char func_names[MAX_NR_OF_FUNCS][MAX_STRING_SIZE];
int func_flops[MAX_NR_OF_FUNCS];

int nrOfFuncs = 0; // nr. of functions


void benchmark_add_render_func(render_func_prot f, char* name, int flops) {
  functions[nrOfFuncs] = f;
  strcpy(func_names[nrOfFuncs], name);
  func_flops[nrOfFuncs] = flops;
  nrOfFuncs++;
}


void run_perf_benchmarking(SceneContainer sceneContainer) {
  for (unsigned int i = 0; i < nrOfFuncs; i++) {

    printf("\nStarting benchmark for funcion %s \n", func_names[i]);
    double perf = perf_test(functions[i], func_names[i], func_flops[i], sceneContainer);
    printf("\nPerformance: %f cycles\n", perf);

  }

}

double perf_test(render_func_prot f, char* name, int flops, SceneContainer sceneContainer) {
  printf("\n|Performance testing...");

    // for each scene in the scene container
    for(unsigned int i = 0; i < sceneContainer.num_scenes; ++i){

      // do performance testing on scene i with given parameters
      for (unsigned int n = START_W_RESOLUTION; n <= END_W_RESOLUTION; n += RESOLUTION_STEPS)
      {
        // Resolution settings
        double height_ = n;
        double width_ = n * SCALE_RATIO; 

        // build index name
        char str_res[100]; // Note: maybe too long/short
        sprintf(str_res, "_%d_%d", (int) height_, (int) width_);

        // build path name with index        
        char* path = OUTPUT_PATH;
        char* add_ = "benchmark_";
        char* str1 = _concat(path, add_); // Note: free up str1!
        char* str2 = _concat(str1, (sceneContainer.scenes)[i]->name); // Note: free up str2!
        char* str3 = _concat(str2, str_res);
        char* filename = _concat(str3, ".png"); // Note: free up filename!

        printf("\n||Starting performance on n = %d with resolution: %d x %d", n, (int) height_, (int)  width_);

        for (unsigned int j = 0; j < REPETITIONS; j++)
        {
          printf("\n|||");
          f(*(sceneContainer.scenes)[i], (unsigned int) width_, (unsigned int) height_, filename);

        }

        // Clean-up allocated strings
        free(str1);
        free(str2);
        free(str3);
        free(filename);

      }

      // Clean-up
      for(unsigned int i = 0; i < sceneContainer.num_scenes; ++i){
        destroy_scene(&(*(sceneContainer.scenes)[i]));
      }
    }

  return 0;
}
