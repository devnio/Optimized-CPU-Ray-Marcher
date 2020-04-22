#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
 #include <sys/time.h>
#endif
#include <time.h>

#ifndef SCENE_H_
 #include "geometry/scene.h"
#endif



/* Prototype of render function */
typedef void(*render_func_prot)(Scene , unsigned int, unsigned int);

/*  */
void benchmark_add_render_func(render_func_prot f, char* name, int flops);

void run_benchmarking();

double perf_test(render_func_prot f, char* name, int flops);