#include "geometry/torus.h"

const int nr_torus_params = 2;

/*
Params are:  
- params[0]: r1 squared
- params[1]: r2
-> 2 parameter in total
 */
double sdf_torus(const double vec_p[NR_VEC_ELEMENTS], double params[])
{
    double l = vec_p[0] * vec_p[0] + vec_p[2] * vec_p[2];
    double sqrtL = sqrt(l);
    return sqrt(l + vec_p[1]*vec_p[1] -2*sqrtL + params[0]) - params[1];
}
