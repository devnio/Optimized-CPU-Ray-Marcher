#ifndef TORUS
#define TORUS

#ifndef VEC_3
 #include "vec3.h"
#endif

#include "material.h"

typedef struct
{
    Vec3 c; 
    double r1; // inner radius of disk
    double r2; // outer radous of the torus
    Material mat;
} Torus;

double sdf_torus(Vec3 p, Torus c);

Torus* build_torus(Vec3 c, double r1, double r2, Material mat);

#endif