#include "geometry/plane.h"

const int nr_plane_params = 4;

/*
Params for plane are:  
- params[0..2]: normal of plane
- params[3]: displacement of plane
-> 4 parameters in total
 */
double sdf_plane(double vec_p[NR_VEC_ELEMENTS], double params[])
{
    return (vec_p[0] * params[0] + vec_p[1] * params[1] + vec_p[2] * params[2]) - params[3];
}
