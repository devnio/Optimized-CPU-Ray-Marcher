#ifndef CONE
#define CONE

#ifndef VEC_3
 #include "vec3.h"
#endif
#include "material.h"

typedef struct
{
    Vec3 c; //center
    double r1; //radius down
    double r2; //radium upper part
    double h; //height
    Material mat;
} Cone;

double sdf_cone(Vec3 p, Cone c);

#endif