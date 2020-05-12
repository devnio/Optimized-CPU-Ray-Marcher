#include "geometry/octahedron.h"
#include "utility.h"

const int nr_octahedron_params = 1;

/*
Params are:  
- params[0]: s
-> 1 parameter in total
 */
double sdf_octahedron(Vec3 p, double params[])
{
    vec_abs(&p);
    double m = p.x + p.y + p.z - params[0];
    Vec3 q;

    if (3.0 * p.x < m)
        new_vector(&q, p.x, p.y, p.z);
    else if (3.0 * p.y < m)
        new_vector(&q, p.y, p.z, p.x);
    else if (3.0 * p.z < m)
        new_vector(&q, p.z, p.x, p.y);
    else
        return m * 0.57735027;

    float k = clamp(0.5 * (q.z - q.y + params[0]), 0.0, params[0]);
    new_vector(&q, q.x, q.y - params[0] + k, q.z - k);
    return vec_norm(&q);
}
