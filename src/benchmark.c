#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "config.h"
#include "utility.h"
#include "benchmark/benchmark.h"
#include "benchmark/tsc_x86.h"
#include "camera.h"
#include "lodepng.h"

// ===============================================================
//                      BENCHMARK FUNCTIONS
// ===============================================================

// ================================
//             RENDER
// ================================
void benchmark_render(render_func_prot f, SceneContainer sceneContainer)
{
    printf("\n||  BENCHMARKING [RENDER FUNCTION]  ||\n|| CHECK FLAGS (did you remove -g?) ||\n||           [CLOSE APPS]           ||\n");

    // Create initial directory
    char *dirName = create_dir_with_current_date(OUTPUT_PATH, __func__);
    printf("\ndirName: %s\n", dirName);

    for (unsigned int i = 0; i < sceneContainer.num_scenes; ++i)
    {
        // Create sub-directory for this scene
        char *newPathName = create_dir(dirName, (sceneContainer.scenes)[i]->name);

        // Create txt file for parameters
        char *time_ = malloc(100 + 1);
        create_params_file(time_, (sceneContainer.scenes)[i]->name, dirName);

        // Create txt file for performance measurements, used to check for opening and path.
        FILE *fmeasurem = NULL;
        char measurem_fileName[100];
        sprintf(measurem_fileName, "%s%s", newPathName, "/measurements.csv");
        fmeasurem = fopen(measurem_fileName, "w");
        if (fmeasurem == NULL)
        {
            printf("ERROR: Failed to create measurements.csv file");
        }
        
        fprintf(fmeasurem, "n, ");
        for (int i = 0; i < REPETITIONS; i++) fprintf(fmeasurem, "Run %d, ", i);
        fprintf(fmeasurem, "AVG\n");

        // Get scene  
        Scene scene = *(sceneContainer.scenes)[i];

        // Run benchmark on specified resolutions
        for (unsigned int n = START_H_RESOLUTION; n <= END_H_RESOLUTION; n += RESOLUTION_STEPS)
        {
            // Write n to file
            fprintf(fmeasurem, "%d, ", n);

            // Resolution settings
            unsigned int height_ = n;
            unsigned int width_ = n * SCALE_RATIO;
            unsigned long long cycles = 0;

            // Set up width and height for camera and image creation
            update_width_height(scene.camera, width_, height_);

            // Create image
            create_image(&scene, scene.camera->widthPx, scene.camera->heightPx);

            // Build index and add to path
            char filename[300];
            sprintf(filename, "%s/out_%d_%d.png", newPathName, (unsigned int)height_, (unsigned int)width_);

            printf("\n|||N = %d, res.: %d x %d, file: %s\n||||", n, (int)height_, (int)width_, filename);

            // ===================================================
            //// --- WARM-UP --- ///
            // ===================================================
            for (unsigned int j = 0; j < WARM_UP_REPETITIONS; j++)
            {
                f(scene);
            }
            // ===================================================
            //// --- START PERFORMANCE MEASUREMENT --- ///
            // ===================================================
            myInt64 start, end;
            for (unsigned int j = 0; j < REPETITIONS; j++)
            {
                // Benchmark
                start = start_tsc(); // start timer
                f(scene);
                end = stop_tsc(start); // end timer
                
                // Accumulate
                cycles += end;
                
                // Write to file
                fprintf(fmeasurem, "%lld, ", end);
                fflush(fmeasurem);
            }
            // ===================================================
            //// --- END PERFORMANCE MEASUREMENT --- ///
            // ===================================================
            cycles = cycles / REPETITIONS;
            fprintf(fmeasurem, "%lld\n", cycles);

            // Save image to disk
            save_image_to_disk(&scene, filename);
            
            // Clean up
            destroy_image(&scene);
        }

        // Clean-up allocated strings and handlers
        fclose(fmeasurem);
        free(time_);
        free(newPathName);

        destroy_scene(&scene);
        free((sceneContainer.scenes)[i]);
    }

    free(dirName);
    free(sceneContainer.scenes);
}

// ================================
//             TRACE
// ================================
void benchmark_trace(trace_func_prot f, SceneContainer sceneContainer)
{
    printf("\n||   BENCHMARKING [TRACE FUNCTION]  ||\n|| CHECK FLAGS (did you remove -g?) ||\n||           [CLOSE APPS]           ||\n");

    // Create initial directory
    char *dirName = create_dir_with_current_date(OUTPUT_PATH, __func__);
    printf("\ndirName: %s\n", dirName);

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
        sprintf(measurem_fileName, "%s%s", newPathName, "/measurements.csv");
        fmeasurem = fopen(measurem_fileName, "w");
        if (fmeasurem == NULL)
        {
            printf("ERROR: Failed to create measurements.txt file");
        }

        fprintf(fmeasurem, "n, AVG accum. cycles (run %d times for every px)\n", REPETITIONS);

        // Get scene  
        Scene scene = *(sceneContainer.scenes)[i];

        // Run benchmark on specified resolutions
        for (unsigned int n = START_H_RESOLUTION; n <= END_H_RESOLUTION; n += RESOLUTION_STEPS)
        {
            // Write n to file
            fprintf(fmeasurem, "%d, ", n);

            // Resolution settings
            unsigned int height_ = n;
            unsigned int width_ = n * SCALE_RATIO;

            // Build index and add to path
            char filename[300];
            sprintf(filename, "%s/out_%d_%d.png", newPathName, (unsigned int)height_, (unsigned int)width_);

            printf("\n|||N = %d, res.: %d x %d, file: %s\n||||", n, (int)height_, (int)width_, filename);

            // Set up width and height for camera and image creation
            update_width_height(scene.camera, width_, height_);
            
            // Create image
            create_image(&scene, scene.camera->widthPx, scene.camera->heightPx);

            myInt64 start, end;
            double inv_AA = 1.0 / AA;
            double inv_AA2 = inv_AA / AA;
            Vec3 tot_col;
            Vec3 px_col;
            unsigned long long cycles = 0;

          
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
                            vec_add(&tot_col, &px_col, &tot_col);
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
                    vec_pow_inplace(&px_col, 0.4545);
#endif

                    // save colors computed by trace into current pixel
                    scene.img[y * width_ * 4 + x * 4 + 0] = (unsigned char)(min(1, px_col.x) * 255);
                    scene.img[y * width_ * 4 + x * 4 + 1] = (unsigned char)(min(1, px_col.y) * 255);
                    scene.img[y * width_ * 4 + x * 4 + 2] = (unsigned char)(min(1, px_col.z) * 255);
                    scene.img[y * width_ * 4 + x * 4 + 3] = 255;
                }
            }
            // ===================================================
            //// --- END PERFORMANCE MEASUREMENT --- ///
            // ===================================================
            cycles = cycles / REPETITIONS;

            // Write to file
            fprintf(fmeasurem, "%lld\n", cycles);
            fflush(fmeasurem);

            // Save image to disk 
            save_image_to_disk(&scene, filename);

            // Clean up
            destroy_image(&scene);
        }

        // Clean-up allocated strings and handlers
        fclose(fmeasurem);
        free(time_);
        free(newPathName);

        destroy_scene(&scene);
        free((sceneContainer.scenes)[i]);
    }

    free(dirName);
    free(sceneContainer.scenes);
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
        fprintf(fparam, "\nScene name: %s", scneneName);
        fprintf(fparam, "\nDate: %s", time_);

        fprintf(fparam, "\nWARM_UP_REPETITIONS: %d", WARM_UP_REPETITIONS);
        fprintf(fparam, "\nREPETITIONS: %d", REPETITIONS);
        fprintf(fparam, "\nSTART_H_RESOLUTION: %d", START_H_RESOLUTION);
        fprintf(fparam, "\nEND_H_RESOLUTION: %d", END_H_RESOLUTION);
        fprintf(fparam, "\nRESOLUTION_STEPS: %d", RESOLUTION_STEPS);
        fprintf(fparam, "\nSCALE_RATIO: %f", SCALE_RATIO);
        
        fflush(fparam);
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