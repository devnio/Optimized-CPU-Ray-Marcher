// #include <stdlib.h>
#include "light.h"

PointLight *create_pointlight(double vec_center[NR_VEC_ELEMENTS], double vec_emissionColor[NR_VEC_ELEMENTS])
{
    PointLight *light = malloc(sizeof(PointLight));

    // Instantiate light center
    light->c = (SIMD_VEC*)aligned_alloc(32, sizeof(SIMD_VEC));
    light->c->x = SET1_PD(vec_center[0]);
    light->c->y = SET1_PD(vec_center[1]);
    light->c->z = SET1_PD(vec_center[2]);

    // Instantiate light emission color
    light->emissionColor = (SIMD_VEC*)aligned_alloc(32, sizeof(SIMD_VEC));
    light->emissionColor->x = SET1_PD(vec_emissionColor[0]);
    light->emissionColor->y = SET1_PD(vec_emissionColor[1]);
    light->emissionColor->z = SET1_PD(vec_emissionColor[2]);

    return light;
}