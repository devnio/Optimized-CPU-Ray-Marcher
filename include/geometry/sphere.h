#ifndef SPHERE_H_
#define SPHERE_H_

#include "vec3.h"
/*
Params for plane are:  
- params[0]: radius
-> 1 parameters in total
 */
double sdf_sphere(const double vec_p[NR_VEC_ELEMENTS], double params[]);
extern const int nr_sphere_params;

#endif