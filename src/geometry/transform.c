#include <math.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>

#include "geometry/transform.h"
#include "utility.h"

Transform *new_transform(double vec_center[NR_VEC_ELEMENTS], double vec_orientation[NR_VEC_ELEMENTS])
{
    Transform *transform = (Transform *)malloc(sizeof(Transform));
    
    // TODO: use another method to create these kind of vectors
    SIMD_VEC simd_vec_center;
    simd_vec_center.x = SET1_PD(vec_center[0]);
    simd_vec_center.y = SET1_PD(vec_center[1]);
    simd_vec_center.z = SET1_PD(vec_center[2]);

    transform->center = simd_vec_center;
    
    double a = to_radians(vec_orientation[0]);
    double b = to_radians(vec_orientation[1]);
    double c = to_radians(vec_orientation[2]);

    transform->orientation[0] = cos(a);
    transform->orientation[1] = sin(a);

    transform->orientation[2] = cos(b);
    transform->orientation[3] = sin(b);

    transform->orientation[4] = cos(c);
    transform->orientation[5] = sin(c);

    return transform;
}