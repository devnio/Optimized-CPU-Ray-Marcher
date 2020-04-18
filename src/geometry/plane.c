#include "geometry/plane.h"
#include "vec3.h"

double sdf_plane(Vec3 p, Plane plane)
{
    return vec_dot(p, plane.n) + plane.d;
}

Plane* build_plane(Vec3 n, double d, Material mat){
    Plane* p = (Plane*) malloc(sizeof(Plane));
    p->d=d;
    p->n = n;
    p->mat = mat;
    return p;
}