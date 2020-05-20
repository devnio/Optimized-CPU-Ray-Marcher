#include <math.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>

#include "geometry/transform.h"
#include "utility.h"

Transform *new_transform(float vec_center[NR_VEC_ELEMENTS], float vec_orientation[NR_VEC_ELEMENTS])
{
    Transform *transform = (Transform *)malloc(sizeof(Transform));

    // Importante the struct has to be aligned so also the childs will be
    transform->center = (SIMD_VEC_PS*)aligned_alloc(32, sizeof(SIMD_VEC_PS));
    transform->center->x = SET1_PS(vec_center[0]);
    transform->center->y = SET1_PS(vec_center[1]);
    transform->center->z = SET1_PS(vec_center[2]);

    float a = to_radians(vec_orientation[0]);
    float b = to_radians(vec_orientation[1]);
    float c = to_radians(vec_orientation[2]);

    transform->orientation[0] = cosf(a);
    transform->orientation[1] = sinf(a);

    transform->orientation[2] = cosf(b);
    transform->orientation[3] = sinf(b);

    transform->orientation[4] = cosf(c);
    transform->orientation[5] = sinf(c);

    return transform;
}