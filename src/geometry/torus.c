#include "geometry/torus.h"

const int nr_torus_params = 2;

/*
Params are:  
- params[0]: r1
- params[1]: r2
-> 2 parameter in total
 */
double sdf_torus(double vec_p[NR_VEC_ELEMENTS], double params[])
{
    double length = sqrt(vec_p[0] * vec_p[0] + vec_p[2] * vec_p[2]);

    double v__q[NR_VEC_ELEMENTS];
    v__q[0] = length - params[0];
    v__q[1] = vec_p[1];
    v__q[2] = 0.0; 
    /// TODO: maybe remove third component v__q[2], since not used here

    return sqrt(v__q[0] * v__q[0] + v__q[1] * v__q[1]) - params[1];
}
