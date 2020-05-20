#ifndef LIGHT_H_
#define LIGHT_H_

#include "vec3.h"

typedef struct
{
    SIMD_VEC_PS* c;
    SIMD_VEC_PS* emissionColor;
} PointLight;

PointLight *create_pointlight(float vec_center[NR_VEC_ELEMENTS], float vec_emissionColor[NR_VEC_ELEMENTS]);

#endif