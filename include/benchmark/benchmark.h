#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
 #include <sys/time.h>
#endif
#include <time.h>

#ifndef SCENE_H_
 #include "geometry/scene.h"
#endif

#ifndef SCENE_LOADER_H_
 #include "scene_loader.h"
#endif

#ifdef _WIN32
# define  mkdir( D, M ) mkdir( D )
#endif


enum type {NOTYPE, RENDER, TRACE};


/* Prototype of render function */
typedef void(*render_func_prot)(Scene, char*);

/* Prototype of trace function */
typedef Vec3(*trace_func_prot)(Vec3, Vec3, Scene, int);

typedef void(*func_pointer)();

// typedef struct fpb {
//     enum {RENDER, TRACE} type;   
//     func_pointer fp;            
// } fbp;

/* Adds rendering type functions to the benchmarker  */
void benchmark_add_render_func(render_func_prot f, char* name, int flops);

void benchmark_add_trace_func(trace_func_prot f, char* name, int flops);

/* Runs performance benchamrkin */
void run_perf_benchmarking(SceneContainer sceneContainer);

/* Performance tester for rendering fucntions */
double perf_test_render(render_func_prot f, char* name, int flops, SceneContainer sceneContainer, char* dirName);

double perf_test_trace(trace_func_prot f, char* name, int flops, SceneContainer sceneContainer, char* dirName);

void create_params_file(char* time, char* scneneName, char* dirName);

char* create_dir(char* path, char* dirName);
void write_measurm_to_file(FILE* f, char* s_n, unsigned int n, char* s_cycles, double cycles, char* s_perf, double perf);