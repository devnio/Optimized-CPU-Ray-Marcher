#include "geometry/plane.h"

const int nr_plane_params = 4;

/*
Params for plane are:  
- params[0..2]: normal of plane
- params[3]: displacement of plane
-> 4 parameters in total
 */
double sdf_plane(Vec3 p, Transform transform, double params[])
{
    return (p.x*params[0] + p.y*params[1] + p.z*params[2]) + params[3];
}
