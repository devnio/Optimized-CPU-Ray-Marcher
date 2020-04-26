#include "material.h"

Material *new_material(Vec3 surfCol, double refl, float shininess)
{
    Material *mat = (Material *)malloc(sizeof(Material));
    mat->surfCol = surfCol;
    mat->refl = refl;
    mat->shininess = shininess;
    return mat;
}