#ifndef LIGHT_H_
#define LIGHT_H_

#include "vec3.h"

typedef struct
{
    SIMD_VEC* c;
    SIMD_VEC* emissionColor;
} PointLight;

PointLight *create_pointlight(double vec_center[NR_VEC_ELEMENTS], double vec_emissionColor[NR_VEC_ELEMENTS]);

#endif