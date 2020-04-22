#ifndef MATERIAL
#define MATERIAL

#include "vec3.h"

typedef struct
{
    Vec3 surfCol;
    double refl;
    float shininess;
    Vec3 emissionColor;
} Material;

Material* new_material(Vec3 surfCol, double refl, float shininess, Vec3 emissionColor);

#endif