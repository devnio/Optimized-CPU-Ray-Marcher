#include "geometry/sphere.h"

const int nr_sphere_params = 1;

/*
Params for plane are:  
- params[0]: radius
-> 1 parameters in total
 */
double sdf_sphere(Vec3 p, Transform transform, double params[])
{
    return vec_norm(vec_sub(transform.center, p)) - params[0];
}

