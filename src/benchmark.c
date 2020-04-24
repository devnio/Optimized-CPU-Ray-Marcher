#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utility.h"
#include "benchmark/benchmark.h"
#include "benchmark/tsc_x86.h"
// #ifndef TSC_X86_H
//   #include "benchmark/tsc_x86.h"
// #endif

// --- Benchmarking and Performance Parameters --- //
#define CYCLES_REQUIRED 1e7
#define REPETITIONS 1
#define FLOPS (4.*n)
#define EPS (1e-3)
#define NUM_RUNS 1
#define FREQUENCY 2.7e9
#define CALIBRATE
#define NR_OF_SAMPLES 30

#define START_W_RESOLUTION 10 // width resolution with which we want to start with
#define END_W_RESOLUTION 40 // width resolution we want to reach
#define RESOLUTION_STEPS 10
#define SCALE_RATIO 1.77777778f // scale ratio width to height of image

#define MAX_NR_OF_FUNCS 32
#define MAX_STRING_SIZE 100

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

  // TODO create directory for each benchmarking
  // char *pathName = malloc(length + 1); 


  // build current date and time to append to path name
  char date_[100];
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(date_, sizeof(date_)-1, "benchmark_%d_%m_%Y_%H_%M_%S", t); 

  // int ret = chdir(OUTPUT_PATH);
  
  char dirName[200] = OUTPUT_PATH;
  strcat(dirName, date_);
  struct stat st = {0};
  if (stat(dirName, &st) == -1) {
    mkdir(dirName, 0700); // create directory 
  } else // directory already exists 
  {
      printf("\nToo fast...directory already exists:  %s", dirName);
  }
  
  printf("\n%s", dirName);
  // ret = chdir(dirName);

  for (unsigned int i = 0; i < nrOfFuncs; i++) {

    // create subdirectory for each function i
    char indx[10];
    sprintf(indx, "_%d", i);
    char* new_subdir_name = _concat(func_names[i], indx); 
    char* new_subdir_name_ = _concat("/", new_subdir_name); 
    char* newDirName = _concat(dirName, new_subdir_name_);
    struct stat st = {0};
    if (stat(newDirName, &st) == -1) { // create subdirectory for function i
      mkdir(newDirName, 0700); // create directory 
    }    


    printf("\n%s", newDirName);

    // strcat(dirName, indx);
    // printf("\n%s", dirName);

    printf("\n|Starting benchmark for function %s", func_names[i]);
    double perf = perf_test(functions[i], func_names[i], func_flops[i], sceneContainer, newDirName);
    printf("\nPerformance: %f cycles\n", perf);

    free(new_subdir_name);
    free(new_subdir_name_);
    free(newDirName);
  }

  // Clean-up
  for(unsigned int i = 0; i < sceneContainer.num_scenes; ++i){
    destroy_scene(&(*(sceneContainer.scenes)[i]));
  }

}

double perf_test(render_func_prot f, char* name, int flops, SceneContainer sceneContainer, char* dirName) {
  printf("\n||Performance testing...");

  double cycles, perf;

  // for each scene in the scene container
  for(unsigned int i = 0; i < sceneContainer.num_scenes; ++i){

    // create sub-directory for each scene
    char indx[10];
    sprintf(indx, "_%d", i);
    printf("\nindex: %s", indx);
    printf("\nScene name: %s", (sceneContainer.scenes)[i]->name);
    char* new_subdir_name = _concat((sceneContainer.scenes)[i]->name, indx); // free
    printf("\nnew_subdir_name: %s", new_subdir_name);
    char* new_subdir_name_ = _concat("/", new_subdir_name);  // free
    printf("\nnew_subdir_name_: %s", new_subdir_name_);
    char* newDirName = _concat(dirName, new_subdir_name_); // free
    printf("\nnewDirName: %s", newDirName);
    struct stat st = {0};
    if (stat(newDirName, &st) == -1) { // create subdirectory for function i
      mkdir(newDirName, 0700); // create directory 
    }  

    // printf("\nDirectory Name: %s", newDirName);


    // TODO: create different path to new subdirectiory for different scenes

    // Create txt file for parameters
    // FILE *fparam = NULL;
    // char fileName[100];
    // sprintf(fileName, "%s%s", OUTPUT_PATH, "parameters.txt");
    // printf("Parameter filename: %s", fileName);
    // fparam = fopen(fileName ,"w");
    // if (fparam != NULL)
    // {
    //   char str1[100] = 
    //   strcat(str1, (sceneContainer.scenes)[i]->name);
    //   fputs(str1, fparam);
    //   fclose(fparam);
    // } else
    // {
    //   printf("WARNING: could not open/create .txt file for writing parameters.");
    // }

    // char text[100];
    // time_t now = time(NULL);
    // struct tm *t = localtime(&now);
    // strftime(text, sizeof(text)-1, "%d %m %Y %H:%M", t);
    // printf("Current Date: %s", text);

    // create txt file for performance timings

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
      char* path = _concat(newDirName, "/"); // free
      printf("\nNew Path Name: %s", path);
      char* add_ = "benchmark_";
      char* str1 = _concat(path, add_); // Note: free up str1!
      char* str2 = _concat(str1, (sceneContainer.scenes)[i]->name); // Note: free up str2!
      char* str3 = _concat(str2, str_res); // Note: free up str3!
      char* filename = _concat(str3, ".png"); // Note: free up filename!

      printf("\n|||Starting performance on n = %d with resolution: %d x %d", n, (int) height_, (int)  width_);

      // TODO: Warm-up phase

      myInt64 start, end;
      start = start_tsc(); // start timer
      for (unsigned int j = 0; j < REPETITIONS; j++)
      {
        f(*(sceneContainer.scenes)[i], (unsigned int) width_, (unsigned int) height_, filename);
      }
      end = stop_tsc(start); // end timer

      cycles = ((double)end) / REPETITIONS;
      perf = flops / cycles;

      // TODO: create cycles list and performance list and add timings or create txt file

      // Clean-up allocated strings and handlers
      free(path);
      free(str1);
      free(str2);
      free(str3);
      free(filename);

    }

    free(newDirName);
    free(new_subdir_name_);
    free(new_subdir_name);
  }
  return cycles;
}

// double perf_test(comp_func_1 f, char* describtion, int flops)
// {
//   double cycles = 0.;
//   double perf = 0.0;
//   long num_runs = 16;
//   double multiplier = 1;
//   myInt64 start, end;

//   double *w, *x, *y, *z;
//   int n = 1000;

//   build(&w, n);
//   build_x(&x, n, n);
//   build(&y, n*n);
//   build(&z, n);

//   // Warm-up phase: we determine a number of executions that allows
//   // the code to be executed for at least CYCLES_REQUIRED cycles.
//   // This helps excluding timing overhead when measuring small runtimes.
//   do {
//     num_runs = num_runs * multiplier;
//     start = start_tsc();
//     for (size_t i = 0; i < num_runs; i++) {
//       f(w,x,y,z,n);      
//     }
//     end = stop_tsc(start);

//     cycles = (double)end;
//     multiplier = (CYCLES_REQUIRED) / (cycles);
    
//   } while (multiplier > 2);

//   list< double > cyclesList, perfList;

//   // Actual performance measurements repeated REP times.
//   // We simply store all results and compute medians during post-processing.
//   for (size_t j = 0; j < REP; j++) {

//     start = start_tsc();
//     for (size_t i = 0; i < num_runs; ++i) {
//       f(w,x,y,z,n);
//     }
//     end = stop_tsc(start);

//     cycles = ((double)end) / num_runs;

//     cyclesList.push_back(cycles);
//     perfList.push_back(FLOPS / cycles);
//   }

//   destroy(w);
//   destroy(x);
//   destroy(y);
//   destroy(z);
//   cyclesList.sort();
//   cycles = cyclesList.front();  
//   return  cycles;
// }