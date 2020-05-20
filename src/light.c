// #include <stdlib.h>
#include "light.h"

PointLight *create_pointlight(float vec_center[NR_VEC_ELEMENTS], float vec_emissionColor[NR_VEC_ELEMENTS])
{
    PointLight *light = malloc(sizeof(PointLight));

    // Instantiate light center
    light->c = (SIMD_VEC_PS*)aligned_alloc(32, sizeof(SIMD_VEC_PS));
    light->c->x = SET1_PS(vec_center[0]);
    light->c->y = SET1_PS(vec_center[1]);
    light->c->z = SET1_PS(vec_center[2]);

    // Instantiate light emission color
    light->emissionColor = (SIMD_VEC_PS*)aligned_alloc(32, sizeof(SIMD_VEC_PS));
    light->emissionColor->x = SET1_PS(vec_emissionColor[0]);
    light->emissionColor->y = SET1_PS(vec_emissionColor[1]);
    light->emissionColor->z = SET1_PS(vec_emissionColor[2]);

    return light;
}