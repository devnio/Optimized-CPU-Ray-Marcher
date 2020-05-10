#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "vec3.h"

typedef struct
{
    Vec3 center;
    double orientation[6];
} Transform;

Transform *new_transform(Vec3 center, Vec3 orientation);

#endif // TRANSFORM_H_