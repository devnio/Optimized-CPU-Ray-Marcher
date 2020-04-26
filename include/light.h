#ifndef LIGHT_H_
#define LIGHT_H_

#include "vec3.h"

typedef struct
{
    Vec3 c;
    Vec3 emissionColor;
} PointLight;

PointLight *create_pointlight(Vec3 center, Vec3 emissionColor);

#endif