// #include <stdlib.h>
#include "light.h"

PointLight *create_pointlight(Vec3 center, Vec3 emissionColor)
{
    PointLight *light = malloc(sizeof(PointLight));
    light->c = center;
    light->emissionColor = emissionColor;
    return light;
}