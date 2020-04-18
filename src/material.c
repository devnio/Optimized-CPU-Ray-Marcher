#include "material.h"

Material new_material(Vec3 surfCol, double refl, float shininess, Vec3 emissionColor){
    Material mat;
    mat.surfCol = surfCol;
    mat.refl = refl;
    mat.shininess = shininess;
    mat.emissionColor = emissionColor;
    return mat;

}