#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "vec3.h"
#include "simd.h"

typedef struct
{
    SIMD_VEC_PS* center; 
    float orientation[6];
} Transform;

Transform *new_transform(float vec_center[NR_VEC_ELEMENTS], float vec_orientation[NR_VEC_ELEMENTS]);

#endif // TRANSFORM_H_