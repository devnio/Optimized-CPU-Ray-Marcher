
#ifndef CONFIG_H_
#define CONFIG_H_

//===============================
//            MACROS
//===============================
// DEBUG
#define DEBUG_MODE 0

#define OUTPUT_PATH "../output"
#define RENDER_OUT "../output/render_out/"

// =========================
//     BENCHMARK PARAMETERS
// =========================
#define CYCLES_REQUIRED 1e7
#define REPETITIONS 5
#define WARM_UP_REPETITIONS 1
#define FLOPS (4. * n)
#define EPS (1e-3)
#define NUM_RUNS 1
#define FREQUENCY 2.7e9
#define CALIBRATE
#define NR_OF_SAMPLES 30

#define START_H_RESOLUTION 100 // width resolution with which we want to start with
#define END_H_RESOLUTION 200   // width resolution we want to reach
#define RESOLUTION_STEPS 100
#define SCALE_RATIO 1.77777778f // scale ratio width to height of image

// =========================
//     RENDER PARAMETERS
// =========================
// RENDERING
#define MAX_RAY_DEPTH 3              // max nr. bounces
#define MARCH_COUNT 3000             // max marching steps
#define BBOX_AXES 100                // bounding box size
#define INTERSECT_THRESHOLD 0.000001 // careful with this -> should be low enoguh for shadow to work

// SHADING
#define SPECULAR_COEFF 0.2
#define MATERIAL_AMBIENT_COEFF 0.01
#define REFLECTIVE_COEFF 0.1
#define SHADOW_LIGHTNESS 0.0
#define LIGHT_STR 3

// FOG
#define FOG 1
#define FOG_COEFF -0.000005

// PRECISION
#define EPSILON 0.001
#define EPSILON_NORMALS 0.0001

// ANTI ALIASING
#define AA 1

//Infinite repetition
#define INFINITE_REP 0

// GAMMA CORRECTION
#define GAMMA_CORR 1



#endif // CONFIG_H_