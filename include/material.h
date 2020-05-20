#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vec3.h"

typedef struct
{
    float surfCol[NR_VEC_ELEMENTS];
    float refl;
    float shininess;
} Material;

Material *new_material(float surfCol[NR_VEC_ELEMENTS], float refl, float shininess);

#endif // MATERIAL_H_