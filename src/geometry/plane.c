#include "geometry/plane.h"
#include "vec3.h"

double sdf_plane(Vec3 p, Plane plane)
{
    return vec_dot(p, plane.n) + plane.d;
}