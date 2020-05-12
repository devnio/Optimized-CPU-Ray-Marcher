// #include <stdlib.h>
#include "light.h"

PointLight *create_pointlight(double vec_center[NR_VEC_ELEMENTS], double vec_emissionColor[NR_VEC_ELEMENTS])
{
    PointLight *light = malloc(sizeof(PointLight));

    light->c[0] = vec_center[0];
    light->c[1] = vec_center[1];
    light->c[2] = vec_center[2];

    light->emissionColor[0] = vec_emissionColor[0];
    light->emissionColor[1] = vec_emissionColor[1];
    light->emissionColor[2] = vec_emissionColor[2];
    
    return light;
}