#include "geometry/torus.h"

const int nr_torus_params = 2;

/*
Params are:  
- params[0]: r1
- params[1]: r2
-> 2 parameter in total
 */
double sdf_torus(Vec3 p, double params[])
{
    double l = p.x * p.x + p.z * p.z;
    double sqrtL = sqrt(l);
    return sqrt(l + p.y*p.y -2*sqrtL + params[0]*params[0]) - params[1];
}
