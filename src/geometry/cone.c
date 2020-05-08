#include "geometry/cone.h"
#include "utility.h"

const int nr_cone_params = 3;

/*
Params are:  
- params[0]: r1
- params[1]: r2
- params[2]: h
-> 3 parameters in total
 */
double sdf_cone(Vec3 p, double params[])
{
    Vec3 q = new_vector(sqrt(p.x * p.x + p.z * p.z), p.y, 0.0);
    Vec3 k1 = new_vector(params[1], params[2], 0.);
    Vec3 k2 = new_vector(params[1] - params[0], 2. * params[2], 0);
    Vec3 ca = new_vector(q.x - fmin(q.x, (q.y < 0.) ? params[0] : params[1]), fabs(q.y) - params[2], 0.);
    Vec3 tmp;
    vec_mult_scalar(&k2, clamp(vec_dot(vec_sub(k1, q), k2) / vec_dot(k2, k2), 0.0, 1.0), &tmp);
    Vec3 cb;
    Vec3 v_sub = vec_sub(q, k1);
    vec_add(&v_sub, &tmp, &cb);
    double s = (cb.x < 0.0 && ca.y < 0.0) ? -1. : 1.0;
    return s * sqrt(fmin(vec_dot(ca, ca), vec_dot(cb, cb)));
}
