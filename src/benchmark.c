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

// ================================
//     BENCHMARK PARAMETERS
// ================================
#define CYCLES_REQUIRED 1e7
#define REPETITIONS 1
#define WARM_UP_REPETITIONS 0
#define FLOPS (4. * n)
#define EPS (1e-3)
#define NUM_RUNS 1
#define FREQUENCY 2.7e9
#define CALIBRATE
#define NR_OF_SAMPLES 30

#define START_H_RESOLUTION 100 // width resolution with which we want to start with
#define END_H_RESOLUTION 500   // width resolution we want to reach
#define RESOLUTION_STEPS 100
#define SCALE_RATIO 1.77777778f // scale ratio width to height of image

#define MAX_NR_OF_FUNCS 32
#define MAX_STRING_SIZE 100

// ===============================================================
//                      BENCHMARK FUNCTIONS
// ===============================================================

// ================================
//             RENDER
// ================================
double benchmark_render(render_func_prot f, SceneContainer sceneContainer)
{
    printf("\n||BENCHMARKING [RENDER FUNCTION]:");

    // Create initial directory
    char *dirName = create_dir_with_current_date(OUTPUT_PATH, __func__);
    printf("\ndirName: %s\n", dirName);
    double cycles;

    for (unsigned int i = 0; i < sceneContainer.num_scenes; ++i)
    {
        // Create sub-directory for this scene
        char *newPathName = create_dir(dirName, (sceneContainer.scenes)[i]->name);

        // Create txt file for parameters
        char *time_ = malloc(100 + 1);
        create_params_file(time_, (sceneContainer.scenes)[i]->name, dirName);

        // Create txt file for performance measurements
        FILE *fmeasurem = NULL;
        char measurem_fileName[100];
        sprintf(measurem_fileName, "%s%s", newPathName, "/measurements.txt");
        fmeasurem = fopen(measurem_fileName, "w");
        if (fmeasurem == NULL)
        {
            printf("ERROR: Failed to create measurements.txt file");
        }

        // Get scene  
        Scene scene = *(sceneContainer.scenes)[i];

        // Run benchmark on specified resolutions
        for (unsigned int n = START_H_RESOLUTION; n <= END_H_RESOLUTION; n += RESOLUTION_STEPS)
        {
            // Resolution settings
            unsigned int height_ = n;
            unsigned int width_ = n * SCALE_RATIO;

            // Set up width and height for camera and image creation
            update_width_height(scene.camera, width_, height_);

            // Build index and add to path
            char filename[300];
            sprintf(filename, "%s/out_%d_%d.png", newPathName, (unsigned int)height_, (unsigned int)width_);

            printf("\n|||Starting performance on n = %d with resolution: %d x %d , filename: %s\n||||", n, (int)height_, (int)width_, filename);

            // ===================================================
            //// --- WARM-UP --- ///
            // ===================================================
            for (unsigned int j = 0; j < WARM_UP_REPETITIONS; j++)
            {
                f(scene, filename);
            }
            // ===================================================
            //// --- START PERFORMANCE MEASUREMENT --- ///
            // ===================================================
            myInt64 start, end;
            start = start_tsc(); // start timer
            for (unsigned int j = 0; j < REPETITIONS; j++)
            {
                f(scene, filename);
            }
            end = stop_tsc(start); // end timer

            // Compute result
            cycles = end / REPETITIONS;
            // ===================================================
            //// --- END PERFORMANCE MEASUREMENT --- ///
            // ===================================================

            // Update measurement.txt file with new measurements
            write_measurm_to_file(fmeasurem, "\nn = ", n, " | Cycles = ", (unsigned long long)cycles); // write measurements to file
        }

        // Clean-up allocated strings and handlers
        fclose(fmeasurem);
        free(time_);
        free(newPathName);
        free(dirName);

        destroy_scene(&scene);
        free((sceneContainer.scenes)[i]);
    }

    return cycles;
}

// ================================
//             TRACE
// ================================
double benchmark_trace(trace_func_prot f, SceneContainer sceneContainer)
{
    printf("\n||BENCHMARKING [TRACE FUNCTION]:");

    // Create initial directory
    char *dirName = create_dir_with_current_date(OUTPUT_PATH, __func__);
    printf("\ndirName: %s\n", dirName);
    double cycles;

    for (unsigned int i = 0; i < sceneContainer.num_scenes; ++i)
    {
        // Create sub-directory for this scene
        char *newPathName = create_dir(dirName, (sceneContainer.scenes)[i]->name);

        // Create txt file for parameters
        char *time_ = malloc(100 + 1);
        create_params_file(time_, (sceneContainer.scenes)[i]->name, dirName);

        // Create txt file for performance measurements
        FILE *fmeasurem = NULL;
        char measurem_fileName[100];
        sprintf(measurem_fileName, "%s%s", newPathName, "/measurements.txt");
        fmeasurem = fopen(measurem_fileName, "w");
        if (fmeasurem == NULL)
        {
            printf("ERROR: Failed to create measurements.txt file");
        }

        // Get scene  
        Scene scene = *(sceneContainer.scenes)[i];

        // Run benchmark on specified resolutions
        for (unsigned int n = START_H_RESOLUTION; n <= END_H_RESOLUTION; n += RESOLUTION_STEPS)
        {
            // Resolution settings
            unsigned int height_ = n;
            unsigned int width_ = n * SCALE_RATIO;


            // Build index and add to path
            char filename[300];
            sprintf(filename, "%s/out_%d_%d.png", newPathName, (unsigned int)height_, (unsigned int)width_);

            printf("\n|||Starting performance on n = %d with resolution: %d x %d , filename: %s\n||||", n, (int)height_, (int)width_, filename);

            // Set up width and height for camera and image creation
            update_width_height(scene.camera, width_, height_);
            create_image(&scene, width_, height_);
            myInt64 start, end;
            double inv_AA = 1.0 / AA;
            double inv_AA2 = inv_AA / AA;
            Vec3 tot_col;
            Vec3 px_col;
            cycles = 0;

          
            // ===================================================
            //// ---  CODE --- ///
            // ===================================================
            for (unsigned y = 0; y < height_; y++)
            {
                for (unsigned x = 0; x < width_; x++)
                {
#if AA > 1
                    tot_col = new_vector(0, 0, 0);
                    for (int m = 0; m < AA; m++)
                    {
                        for (int n = 0; n < AA; n++)
                        {
                            // pixel coordinates
                            double disp_x = (inv_AA * n - 0.5) + x;
                            double disp_y = (inv_AA * m - 0.5) + y;
                            Vec3 dir = shoot_ray(scene.camera, disp_x, disp_y);
                            // ===================================================
                            //// --- START PERFORMANCE MEASUREMENT --- ///
                            // ===================================================
                            start = start_tsc(); // start timer
                            for (unsigned int j = 0; j < REPETITIONS; j++)
                            {
                                px_col = f(scene.camera->pos, dir, scene, 0);
                            }
                            end = stop_tsc(start); // end timer
                            cycles += end * inv_AA2;
                            // ===================================================
                            //// --- END PERFORMANCE MEASUREMENT --- ///
                            // ===================================================
                            tot_col = vec_add(tot_col, px_col);
                        }
                    }
                    Vec3 px_col = vec_mult_scalar(tot_col, inv_AA2);
#else
                    Vec3 dir = shoot_ray(scene.camera, x, y);
                    // ===================================================
                    //// --- START PERFORMANCE MEASUREMENT --- ///
                    // ===================================================
                    start = start_tsc(); // start timer
                    for (unsigned int j = 0; j < REPETITIONS; j++)
                    {
                        px_col = f(scene.camera->pos, dir, scene, 0);
                    }
                    end = stop_tsc(start); // end timer
                    cycles += end;
                    // ===================================================
                    //// --- END PERFORMANCE MEASUREMENT --- ///
                    // ===================================================
#endif

#if GAMMA_CORR == 1
                    px_col = vec_pow(px_col, 0.4545);
#endif

                    // save colors computed by trace into current pixel
                    scene.img[y * width_ * 4 + x * 4 + 0] = (unsigned char)(min(1, px_col.x) * 255);
                    scene.img[y * width_ * 4 + x * 4 + 1] = (unsigned char)(min(1, px_col.y) * 255);
                    scene.img[y * width_ * 4 + x * 4 + 2] = (unsigned char)(min(1, px_col.z) * 255);
                    scene.img[y * width_ * 4 + x * 4 + 3] = 255;
                }
            }
            cycles = cycles / REPETITIONS;

            // ===================================================
            //// --- END PERFORMANCE MEASUREMENT --- ///
            // ===================================================
            
            encodeOneStep(filename, scene.img, width_, height_);
            
            // Update measurement.txt file with new measurements
            write_measurm_to_file(fmeasurem, "\nn = ", n, " | Cycles = ", (unsigned long long)cycles); // write measurements to file
        }

        // Clean-up allocated strings and handlers
        fclose(fmeasurem);
        free(time_);
        free(newPathName);
        free(dirName);

        destroy_scene(&scene);
        free((sceneContainer.scenes)[i]);
    }

    return cycles;
}

// ================================
//        FILE MANAGEMENT
// ================================
void get_current_date(char *out_date)
{
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(out_date, 100, "%d_%m_%Y_%H_%M_%S", t);
}

void write_measurm_to_file(FILE *f, char *s_n, unsigned int n, char *s_cycles, unsigned long long cycles)
{
    const unsigned int bufferLength = 1000;
    char idx[20];
    char txt_measur[bufferLength];
    txt_measur[0] = '\0';

    strcat(txt_measur, s_n);
    sprintf(idx, "%d", n);
    strcat(txt_measur, idx);

    strcat(txt_measur, s_cycles);
    sprintf(idx, "%lld", cycles);
    strcat(txt_measur, idx);

    fputs(txt_measur, f);
}

void create_params_file(char *time_, char *scneneName, char *dirName)
{

    char param_fileName[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(time_, 100, "%d.%m.%Y %H:%M", t);

    FILE *fparam = NULL;
    sprintf(param_fileName, "%s%s", dirName, "/parameters.txt");
    fparam = fopen(param_fileName, "w");
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

        strcat(out_text, "\nSTART_H_RESOLUTION: ");
        sprintf(tmp, "%d", START_H_RESOLUTION);
        strcat(out_text, tmp);

        strcat(out_text, "\nEND_H_RESOLUTION: ");
        sprintf(tmp, "%d", END_H_RESOLUTION);
        strcat(out_text, tmp);

        strcat(out_text, "\nRESOLUTION_STEPS: ");
        sprintf(tmp, "%d", RESOLUTION_STEPS);
        strcat(out_text, tmp);

        strcat(out_text, "\nSCALE_RATIO: ");
        sprintf(tmp, "%f", SCALE_RATIO);
        strcat(out_text, tmp);

        fputs(out_text, fparam);
        fclose(fparam);
    }
    else
    {
        printf("ERROR: Failed to create parameters.txt file");
    }
}

char *create_dir_with_current_date(char *path, const char* prepend)
{
    char date[100];
    get_current_date(date);

    char dirName[500];
    sprintf(dirName, "%s_%s", prepend, date);

    return create_dir(path, dirName); // NOTE: need to free later.
}

/* 
    Creates a sub-directory from path and returns res = path/dirName
*/
char *create_dir(char *path, char *dirName)
{
    char *newDirName = _concat("/", dirName);
    char *newPathName = _concat(path, newDirName);
    struct stat st = {0};

    if (stat(newPathName, &st) == -1)
    {
        mkdir(newPathName, 0700);
    }
    free(newDirName);

    return newPathName; // NOTE: need to free later.
}