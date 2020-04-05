#ifndef SPHERE
#define SPHERE

#ifndef VEC_3
 #include "vec3.h"
#endif
#include "material.h"

typedef struct
{
    Vec3 c;
    double r;
    double r2;
    Material mat;
} Sphere;

double sphere_ray_intersection(Vec3 o, Vec3 dir, Sphere s);
double sdf_sphere(Vec3 p, Sphere s);

#endif