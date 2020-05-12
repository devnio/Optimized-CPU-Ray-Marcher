#include "stdio.h"
#include "geometry/sphere.h"

const int nr_sphere_params = 1;

/*
Params for plane are:  
- params[0]: radius
-> 1 parameters in total
 */
double sdf_sphere(double vec_p[NR_VEC_ELEMENTS], double params[])
{
    return vec_norm(vec_p) - params[0];
}
