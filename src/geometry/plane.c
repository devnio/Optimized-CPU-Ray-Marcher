#include "geometry/plane.h"
#include "vec3.h"

// params[0..2]: normal of plane
// params[3]: displacement of plane
double sdf_plane(Vec3 p, Transform transform, double params[])
{
    return (p.x*params[0] + p.y*params[1] + p.z*params[2]) + params[3];
}
