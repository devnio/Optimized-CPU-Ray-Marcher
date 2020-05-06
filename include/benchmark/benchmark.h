#include <stdio.h>
#include <stdlib.h>
#ifndef WIN32
 #include <sys/time.h>
#endif
#include <time.h>

#include "geometry/scene.h"
#include "scene_loader.h"

#ifdef _WIN32
# define  mkdir( D, M ) mkdir( D )
#endif

// Function pointers to functions that we benchmark
typedef void(*render_func_prot)(Scene, char*);
typedef Vec3(*trace_func_prot)(Vec3, Vec3, Scene, int);

// Benchmark functions
double benchmark_render(render_func_prot f, SceneContainer sceneContainer);
double benchmark_trace(trace_func_prot f, SceneContainer sceneContainer);

void create_params_file(char* time, char* scneneName, char* dirName);
char* create_dir_with_current_date(char *path, const char* prepend);
char* create_dir(char* path, char* dirName);
void write_measurm_to_file(FILE *f, char *s_n, unsigned int n, char *s_cycles, unsigned long long cycles);
