#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vec3.h"

typedef struct
{
    double surfCol[NR_VEC_ELEMENTS];
    double refl;
    float shininess;
} Material;

Material *new_material(double surfCol[NR_VEC_ELEMENTS], double refl, float shininess);

#endif // MATERIAL_H_