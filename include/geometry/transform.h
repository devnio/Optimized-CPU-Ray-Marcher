#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "vec3.h"
#include "simd.h"

typedef struct
{
    SIMD_VEC* center; 
    double orientation[6];
} Transform;

Transform *new_transform(double vec_center[NR_VEC_ELEMENTS], double vec_orientation[NR_VEC_ELEMENTS]);

#endif // TRANSFORM_H_