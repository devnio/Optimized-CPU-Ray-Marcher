#include <math.h>

#include "geometry/transform.h"
#include "utility.h"

Transform *new_transform(double vec_center[NR_VEC_ELEMENTS], double vec_orientation[NR_VEC_ELEMENTS])
{
    Transform *transform = (Transform *)malloc(sizeof(Transform));
    transform->center[0] = vec_center[0];
    transform->center[1] = vec_center[1];
    transform->center[2] = vec_center[2];
    
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