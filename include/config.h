#ifndef CONFIG_H_
#define CONFIG_H_

#include "vec3.h"

//===============================
//            OPTIMIZATION
//===============================
# define FORCE_INLINE __attribute__((always_inline)) inline

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
#define REPETITIONS 4
#define WARM_UP_REPETITIONS 0
// #define NR_OF_SAMPLES 30

#define START_H_RESOLUTION 500 // width resolution with which we want to start with
#define END_H_RESOLUTION 500   // width resolution we want to reach
#define RESOLUTION_STEPS 100
#define SCALE_RATIO 1.77777778f // scale ratio width to height of image

// =========================
//     RENDER PARAMETERS
// =========================
// RENDERING
#define MAX_RAY_DEPTH 3              // max nr. bounces
#define MARCH_COUNT 3000             // max marching steps
#define BBOX_AXES 10000                // bounding box size
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

// Global Constants
extern const double const_specularColour[3];
extern const double const_eps_x[3];
extern const double const_eps_y[3];
extern const double const_eps_z[3];


#endif // CONFIG_H_