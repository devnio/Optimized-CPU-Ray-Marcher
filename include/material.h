#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vec3.h"

typedef struct
{
    Vec3 surfCol;
    double refl;
    float shininess;
} Material;

Material *new_material(Vec3 surfCol, double refl, float shininess);

#endif // MATERIAL_H_