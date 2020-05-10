#include <math.h>

#include "geometry/transform.h"
#include "utility.h"

Transform *new_transform(Vec3 center, Vec3 orientation)
{
    Transform *transform = (Transform *)malloc(sizeof(Transform));
    transform->center = center;
    
    double a = to_radians(orientation.x);
    double b = to_radians(orientation.y);
    double c = to_radians(orientation.z);

    transform->orientation[0] = cos(a);
    transform->orientation[1] = sin(a);

    transform->orientation[2] = cos(b);
    transform->orientation[3] = sin(b);

    transform->orientation[4] = cos(c);
    transform->orientation[5] = sin(c);

    return transform;
}