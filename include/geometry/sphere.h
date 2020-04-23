#ifndef SPHERE
#define SPHERE

#include "vec3.h"
#include "transform.h"

/*
Params for plane are:  
- params[0]: radius
-> 1 parameters in total
 */
double sdf_sphere(Vec3 p, Transform transform, double params[]);
extern const int nr_sphere_params;

#endif