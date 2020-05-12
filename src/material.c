#include "material.h"

Material *new_material(double surfCol[NR_VEC_ELEMENTS], double refl, float shininess)
{
    Material *mat = (Material *)malloc(sizeof(Material));
    mat->surfCol[0] = surfCol[0];
    mat->surfCol[1] = surfCol[1];
    mat->surfCol[2] = surfCol[2];
    mat->refl = refl;
    mat->shininess = shininess;
    return mat;
}