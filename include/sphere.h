#ifndef SPHERE
#define SPHERE

#ifndef VEC_3
 #include "vec3.h"
#endif

typedef struct
{
    Vec3 c;
    double r;
    double r2;
    Vec3 surfCol;
    double refl;
    float shininess;
    Vec3 emissionColor;
} Sphere;

double sphere_ray_intersection(Vec3 o, Vec3 dir, Sphere s);
double sdf(Vec3 p, Sphere s);

#endif