#include "geometry/transform.h"

Transform *new_transform(Vec3 center, Vec3 orientation)
{
    Transform *transform = (Transform *)malloc(sizeof(Transform));
    transform->center = center;
    transform->orientation = orientation;
    return transform;
}