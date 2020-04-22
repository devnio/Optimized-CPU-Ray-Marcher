#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// #ifndef TSC_X86_H
//   #include "benchmark/tsc_x86.h"
// #endif

#include "benchmark/benchmark.h"
#include "benchmark/tsc_x86.h"


#define CYCLES_REQUIRED 1e7
#define REP 10
#define FLOPS (4.*n)
#define EPS (1e-3)
#define NUM_RUNS 1
#define FREQUENCY 2.7e9
#define CALIBRATE
#define NR_OF_SAMPLES 30

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


void run_benchmarking() {
  for (unsigned int i = 0; i < nrOfFuncs; i++)
  {

    printf("\nStarting benchmark for funcion %s \n", func_names[i]);

    double perf = perf_test(functions[i], func_names[i], func_flops[i]);

    printf("\nPerformance: %f cycles\n", perf);
    // cout << perf << " cycles" << std::endl;
  }

}

double perf_test(render_func_prot f, char* name, int flops) {
  return 0;
}
