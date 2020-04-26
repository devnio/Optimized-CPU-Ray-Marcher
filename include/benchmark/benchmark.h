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


/* Prototype of render function */
typedef void(*render_func_prot)(Scene , unsigned int, unsigned int, char*);

/* Prototype of trace function */
typedef void(*trace_func_prot)(Vec3 , Scene, int, SDF_Info *);

typedef void(*func_pointer)();

/* Adds rendering type functions to the benchmarker  */
void benchmark_add_render_func(render_func_prot f, char* name, int flops) ;

/* Runs performance benchamrkin */
void run_perf_benchmarking(SceneContainer sceneContainer);

/* Performance tester for rendering fucntions */
double perf_test(render_func_prot f, char* name, int flops, SceneContainer sceneContainer, char* dirName);