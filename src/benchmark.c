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
#include "camera.h"
#include "lodepng.h"
// #ifndef TSC_X86_H
//   #include "benchmark/tsc_x86.h"
// #endif

// --- Benchmarking and Performance Parameters --- //
#define CYCLES_REQUIRED 1e7
#define REPETITIONS 1
#define WARM_UP_REPETITIONS 2
#define FLOPS (4.*n)
#define EPS (1e-3)
#define NUM_RUNS 1
#define FREQUENCY 2.7e9
#define CALIBRATE
#define NR_OF_SAMPLES 30

#define START_W_RESOLUTION 10 // width resolution with which we want to start with
#define END_W_RESOLUTION 50 // width resolution we want to reach
#define RESOLUTION_STEPS 10
#define SCALE_RATIO 1.77777778f // scale ratio width to height of image

#define MAX_NR_OF_FUNCS 32
#define MAX_STRING_SIZE 100

#define DEBUG_MODE 1
// render function definitions
render_func_prot renderFuncs[MAX_NR_OF_FUNCS]; // array of function pointers
char renderFuncsNames[MAX_NR_OF_FUNCS][MAX_STRING_SIZE];
int renderFuncsFLops[MAX_NR_OF_FUNCS];
int nrOfRenderFuncs = 0; // nr. of functions

// trace function definitions
trace_func_prot traceFuncs[MAX_NR_OF_FUNCS];
char traceFuncsNames[MAX_NR_OF_FUNCS][MAX_STRING_SIZE];
int traceFuncsFLops[MAX_NR_OF_FUNCS];
int nrOfTraceFuncs = 0; // nr. of functions


enum type benchtype = NOTYPE;


// fbp f_pointer;
// void benchmark_add_func(trace_func_prot f, char* name, int flops) {
// }

void benchmark_add_trace_func(trace_func_prot f, char* name, int flops) {
  if (benchtype == NOTYPE)
  {
    benchtype = TRACE;
    printf("\nTrace function added to benchmarker.");
  } else
  {
    printf("\nWARNING: Already function of other type added to benchmarker!");
  }
  traceFuncs[nrOfTraceFuncs] = f;
  strcpy(traceFuncsNames[nrOfTraceFuncs], name);
  traceFuncsFLops[nrOfTraceFuncs] = flops;
  nrOfTraceFuncs++;
}

void benchmark_add_render_func(render_func_prot f, char* name, int flops) {
  if (benchtype == NOTYPE)
  {
    benchtype = RENDER;
    printf("\nRender function added to benchmarker.");
  } else
  {
    printf("\nWARNING: Already function of other type added to benchmarker!");
  }
  renderFuncs[nrOfRenderFuncs] = f;
  strcpy(renderFuncsNames[nrOfRenderFuncs], name);
  renderFuncsFLops[nrOfRenderFuncs] = flops;
  nrOfRenderFuncs++;
}


void run_perf_benchmarking(SceneContainer sceneContainer) {

  // build current date and time to append to path name
  char date_[100];
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(date_, sizeof(date_)-1, "benchmark_%d_%m_%Y_%H_%M_%S", t); 
  
  char dirName[200] = OUTPUT_PATH;
  char bFuncName[100];
  /// TODO: concat function name according to benchtype before "benchmark"
  strcat(dirName, date_);
  struct stat st = {0};
  if (stat(dirName, &st) == -1) {
    mkdir(dirName, 0700); // create directory 
  } else // directory already exists 
  {
      printf("\nToo fast...directory already exists:  %s", dirName);
  }

  unsigned int nrOfFunctions;
  render_func_prot *_funcs; // array of function pointers
  char **_funcsNames;
  int *_funcsFLops;
  if (benchtype == RENDER)
  {
    nrOfFunctions = nrOfRenderFuncs;
  } else if (benchtype == TRACE)
  {
    nrOfFunctions = nrOfTraceFuncs;
  } else
  {
    /* code */
  }
  
  for (unsigned int i = 0; i < nrOfFunctions; i++) {

    // create subdirectory for each function i
    char indx[10];
    sprintf(indx, "_%d", i);
    char* new_subdir_name; 

    if (benchtype == RENDER)
    {
      new_subdir_name = _concat(renderFuncsNames[i], indx);
    } else if (benchtype == TRACE)
    {
      new_subdir_name = _concat(traceFuncsNames[i], indx);
    } else
    {
      /* code */
    }

    char* new_subdir_name_ = _concat("/", new_subdir_name); 
    char* newDirName = _concat(dirName, new_subdir_name_);
    struct stat st = {0};
    if (stat(newDirName, &st) == -1) { 
      mkdir(newDirName, 0700); // create subdirectory for function i
    }    

    // Start benchmark on registered functions


    if (benchtype == RENDER)
    {
      printf("\n|Starting benchmark for render function %s", renderFuncsNames[i]);
      double perf = perf_test_render(renderFuncs[i], renderFuncsNames[i], renderFuncsFLops[i], sceneContainer, newDirName);
      printf("\n|Done.\n");
    } else if (benchtype == TRACE)
    {
      printf("\n|Starting benchmark for trace function %s", traceFuncsNames[i]);
      double perf = perf_test_trace(traceFuncs[i], traceFuncsNames[i], traceFuncsFLops[i], sceneContainer, newDirName);
      printf("\n|Done.\n");
    } else
    {
      /* code */
    }


    free(new_subdir_name);
    free(new_subdir_name_);
    free(newDirName);
  }

  // Clean-up
  for(unsigned int i = 0; i < sceneContainer.num_scenes; ++i){
    destroy_scene(&(*(sceneContainer.scenes)[i]));
  }

}

double perf_test_render(render_func_prot f, char* name, int flops, SceneContainer sceneContainer, char* dirName) {
  printf("\n||Performance testing:");

  double cycles, perf;

  // for each scene in the scene container
  for(unsigned int i = 0; i < sceneContainer.num_scenes; ++i){

    // create sub-directory for each scene
    char indx[10];
    sprintf(indx, "_%d", i);
    char* new_subdir_name = _concat((sceneContainer.scenes)[i]->name, indx); // free
    char* new_subdir_name_ = _concat("/", new_subdir_name);  // free
    char* newDirName = _concat(dirName, new_subdir_name_); // free
    struct stat st = {0};
    if (stat(newDirName, &st) == -1) { // create subdirectory for function i
      mkdir(newDirName, 0700); // create directory 
    }  

    // Create txt file for parameters
    char *time_ = malloc(100 + 1);
    create_params_file(time_, (sceneContainer.scenes)[i]->name, dirName);

    // create txt file for performance timings
    char* path = _concat(newDirName, "/"); // free
    char* add_ = "benchmark_";
    char* str1 = _concat(path, add_); // Note: free up str1!
    char* str2 = _concat(str1, (sceneContainer.scenes)[i]->name); // Note: free up str2!

    // Create txt file for performance measurements
    FILE *fmeasurem = NULL;
    char measurem_fileName[100];
    sprintf(measurem_fileName, "%s%s", newDirName, "/measurements.txt");
    fmeasurem = fopen(measurem_fileName ,"w");
    if (fmeasurem == NULL) {
      printf("ERROR: Failed to create measurements.txt file");
    }
    char txt_measur[100];
    char temp_text[100] = "Scene name: ";
    strcat(temp_text, (sceneContainer.scenes)[i]->name);
    strcat(temp_text, "\nDate: ");
    strcat(temp_text, time_);
    fputs(temp_text, fmeasurem);
  
    // do performance testing on scene i with given parameters
    for (unsigned int n = START_W_RESOLUTION; n <= END_W_RESOLUTION; n += RESOLUTION_STEPS)
    {
      // Resolution settings
      double height_ = n;
      double width_ = n * SCALE_RATIO; 

      // build index and add to path    
      char str_res[100]; 
      sprintf(str_res, "_%d_%d", (unsigned int) height_, (unsigned int) width_); 
      char* str3 = _concat(str2, str_res); // Note: free up str3!
      char* filename = _concat(str3, ".png"); // Note: free up filename!

      printf("\n|||Starting performance on n = %d with resolution: %d x %d \n||||", n, (int) height_, (int)  width_);

      // TODO: Warm-up phase

      myInt64 start, end;
      start = start_tsc(); // start timer

      for (unsigned int j = 0; j < REPETITIONS; j++)
      {
        f(*(sceneContainer.scenes)[i], (unsigned int) width_, (unsigned int) height_, filename);
      }
      end = stop_tsc(start); // end timer

      cycles = ((double)end) / REPETITIONS; 
      perf = flops / cycles; // performance in flops/cycle

      // update measurement.txt file with new measurements
      char idx[20];
      txt_measur[0] = '\0';
      strcat(txt_measur, "\nn = ");
      sprintf(idx, "%d", n); 
      strcat(txt_measur, idx);
      strcat(txt_measur, " | Cycles = ");
      sprintf(idx, "%f", cycles);
      strcat(txt_measur, idx);
      strcat(txt_measur, " | Perf = ");
      sprintf(idx, "%f", perf);
      strcat(txt_measur, idx);
      fputs(txt_measur, fmeasurem);

      // Clean-up allocated strings and handlers
      free(str3);
      free(filename);

    }

    // Clean-up allocated strings and handlers
    fclose(fmeasurem);
    free(str1);
    free(str2);
    free(path);
    free(newDirName);
    free(new_subdir_name_);
    free(new_subdir_name);
  }

  return cycles;
}


double perf_test_trace(trace_func_prot f, char* name, int flops, SceneContainer sceneContainer, char* dirName) {
  printf("\n||Performance testing:");

  double cycles_temp, cycles, perf, cycles_pp, perf_pp;

  // for each scene in the scene container
  for(unsigned int i = 0; i < sceneContainer.num_scenes; ++i){

    // create sub-directory for each scene
    char indx[10];
    sprintf(indx, "_%d", i);
    char* new_subdir_name = _concat((sceneContainer.scenes)[i]->name, indx); // free
    char* new_subdir_name_ = _concat("/", new_subdir_name);  // free
    char* newDirName = _concat(dirName, new_subdir_name_); // free
    struct stat st = {0};
    if (stat(newDirName, &st) == -1) { // create subdirectory for function i
      mkdir(newDirName, 0700); // create directory 
    }  

    // Create txt file for parameters
    char *time_ = malloc(100 + 1);
    create_params_file(time_, (sceneContainer.scenes)[i]->name, dirName);


    // create txt file for performance timings
    char* path = _concat(newDirName, "/"); // free
    char* add_ = "benchmark_";
    char* str1 = _concat(path, add_); // Note: free up str1!
    char* str2 = _concat(str1, (sceneContainer.scenes)[i]->name); // Note: free up str2!

    // Create txt file for performance measurements
    FILE *fmeasurem = NULL;
    char measurem_fileName[100];
    sprintf(measurem_fileName, "%s%s", newDirName, "/measurements.txt");
    fmeasurem = fopen(measurem_fileName ,"w");
    if (fmeasurem == NULL) {
      printf("ERROR: Failed to create measurements.txt file");
    }
    char txt_measur[100];
    char temp_text[100] = "Scene name: ";
    strcat(temp_text, (sceneContainer.scenes)[i]->name);
    strcat(temp_text, "\nDate: ");
    strcat(temp_text, time_);
    fputs(temp_text, fmeasurem);

    // do performance testing on scene i with given parameters
    for (unsigned int n = START_W_RESOLUTION; n <= END_W_RESOLUTION; n += RESOLUTION_STEPS)
    {
      // Resolution settings
      unsigned int height_ = n;
      unsigned int width_ = n * SCALE_RATIO; 

      // build index and add to path    
      char str_res[100]; 
      sprintf(str_res, "_%d_%d", (unsigned int) height_, (unsigned int) width_); 
      char* str3 = _concat(str2, str_res); // Note: free up str3!
      char* filename = _concat(str3, ".png"); // Note: free up filename!

      printf("\n|||Starting performance on n = %d with resolution: %d x %d \n||||", n, (int) height_, (int)  width_);

      float fov = 30;
      Camera* camera = create_camera(fov, width_, height_);

      //Translation and rotation
      Vec3 t = {0.0,5.0,-7};
      move_camera(camera, t);
      rotate_camera(camera, 20, 0);

      // debug
      printf("RENDERING... ");
      float progress = 0.;
      float progress_step = 1./(width_*height_);
      progress += progress_step;

      Vec3 tot_col;

      // render
      size_t png_img_size = width_ * height_ * 4 * sizeof(unsigned char);
      unsigned char *img = (unsigned char *)malloc(png_img_size);
      

      cycles_temp = 0;
      myInt64 start, end;

      for (size_t y = 0; y < height_; y++)
      {
        for (size_t x = 0; x < width_; x++)
        {
          Vec3 px_col;
          Vec3 dir = shoot_ray(camera, x, y);


          //// --- WARM-UP --- ///
          for (unsigned int j = 0; j < WARM_UP_REPETITIONS; j++)
          {
            px_col = f(camera->pos, dir, *(sceneContainer.scenes)[i], 0, NULL);
          }

          //// --- START PERFORMANCE MEASUREMENT --- ///
          start = start_tsc(); // start timer
          for (unsigned int j = 0; j < REPETITIONS; j++)
          {
            px_col = f(camera->pos, dir, *(sceneContainer.scenes)[i], 0, NULL);
          }
          end = stop_tsc(start); // end timer
          cycles_temp += ((double)end) / REPETITIONS;
          //// --- END PERFORMANCE MEASUREMENT --- ///

          // save colors computed by trace into current pixel
          img[y * width_ * 4 + x * 4 + 0] = (unsigned char)(min(1, px_col.x) * 255);
          img[y * width_ * 4 + x * 4 + 1] = (unsigned char)(min(1, px_col.y) * 255);
          img[y * width_ * 4 + x * 4 + 2] = (unsigned char)(min(1, px_col.z) * 255);
          img[y * width_ * 4 + x * 4 + 3] = 255;
            
          if (DEBUG_MODE)
          {
              progress += progress_step;
              fflush(stdout);
              printf(" %.2f\b\b\b\b\b", progress);
              fflush(stdout);
          }
        }
          
      }

      encodeOneStep(filename, img, width_, height_);

      // cycles and performance for rendering whole image with trace
      cycles = cycles_temp; 
      perf = flops / cycles; // performance in flops/cycle

      // cycles and performance per pixel
      cycles_pp = cycles_temp/(height_*height_); 
      perf_pp = flops / cycles_pp; // performance in flops/cycle

      /////////////////////////////



      // update measurement.txt file with new measurements
      char idx[20];
      txt_measur[0] = '\0';
      strcat(txt_measur, "\nn = ");
      sprintf(idx, "%d", n); 
      strcat(txt_measur, idx);
      strcat(txt_measur, " | Cycles = ");
      sprintf(idx, "%f", cycles);
      strcat(txt_measur, idx);
      strcat(txt_measur, " | Perf = ");
      sprintf(idx, "%f", perf);
      strcat(txt_measur, idx);
      fputs(txt_measur, fmeasurem);

      // Clean-up allocated strings and handlers
      free(str3);
      free(filename);

    }

    // Clean-up allocated strings and handlers
    fclose(fmeasurem);
    free(time_);
    free(str1);
    free(str2);
    free(path);
    free(newDirName);
    free(new_subdir_name_);
    free(new_subdir_name);
  }

  return cycles;
}



void create_params_file(char* time_, char* scneneName, char* dirName) {

    char param_fileName[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(time_, 100, "%d.%m.%Y %H:%M", t);
    // strftime(time_, sizeof(time_)-1, "%d.%m.%Y %H:%M", t);
    FILE *fparam = NULL;
    sprintf(param_fileName, "%s%s", dirName, "/parameters.txt");
    fparam = fopen(param_fileName ,"w");
    if (fparam != NULL)
    {
      char tmp[100];
      char out_text[500] = "Scene name: ";
      strcat(out_text, scneneName);
      strcat(out_text, "\nDate: ");
      strcat(out_text, time_);

      strcat(out_text, "\nCYCLES_REQUIRED: ");
      sprintf(tmp, "%f", CYCLES_REQUIRED); 
      strcat(out_text, tmp);

      strcat(out_text, "\nREPETITIONS: ");
      sprintf(tmp, "%d", REPETITIONS); 
      strcat(out_text, tmp);

      strcat(out_text, "\nNUM_RUNS: ");
      sprintf(tmp, "%d", NUM_RUNS); 
      strcat(out_text, tmp);

      strcat(out_text, "\nNR_OF_SAMPLES: ");
      sprintf(tmp, "%d", NR_OF_SAMPLES); 
      strcat(out_text, tmp);

      strcat(out_text, "\nSTART_W_RESOLUTION: ");
      sprintf(tmp, "%d", START_W_RESOLUTION); 
      strcat(out_text, tmp);

      strcat(out_text, "\nEND_W_RESOLUTION: ");
      sprintf(tmp, "%d", END_W_RESOLUTION); 
      strcat(out_text, tmp);

      strcat(out_text, "\nRESOLUTION_STEPS: ");
      sprintf(tmp, "%d", RESOLUTION_STEPS); 
      strcat(out_text, tmp);

      strcat(out_text, "\nSCALE_RATIO: ");
      sprintf(tmp, "%f", SCALE_RATIO); 
      strcat(out_text, tmp);

      fputs(out_text, fparam);
      fclose(fparam);
    } else
    {
      printf("ERROR: Failed to create parameters.txt file");
    }
}