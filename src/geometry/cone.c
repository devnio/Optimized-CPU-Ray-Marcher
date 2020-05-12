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


    // Vec3 k1 = new_vector(params[1], params[2], 0.);
    // Vec3 k2 = new_vector(params[1] - params[0], 2. * params[2], 0);


    Vec3 ca = new_vector(q.x - fmin(q.x, (q.y < 0.) ? params[0] : params[1]), fabs(q.y) - params[2], 0.);
    
    Vec3 tmp, v_sub;
    // vec_sub(&k1, &q, &v_sub);
    v_sub.x = params[1] - q.x;
    v_sub.y = params[2] - q.y;

    // vec_mult_scalar(&k2, clamp(vec_dot(&v_sub, &k2) / vec_dot(&k2, &k2), 0.0, 1.0), &tmp);
    double coeff = clamp(((v_sub.x*params[3] + v_sub.y*params[4]) / params[5]), 0.0, 1.0);
    tmp.x = coeff * params[3];
    tmp.y = coeff * params[4];

    Vec3 cb;
    Vec3 v_sub_2;

    // vec_sub(&q, &k1, &v_sub_2);
    v_sub_2.x = q.x - params[1];
    v_sub_2.y = q.y - params[2];

    // vec_add(&v_sub_2, &tmp, &cb);
    cb.x = v_sub_2.x + tmp.x;
    cb.y = v_sub_2.y + tmp.y;

    double dot_ca = ca.x*ca.x + ca.y*ca.y;
    double dot_cb = cb.x*cb.x + cb.y*cb.y;

    double s = (cb.x < 0.0 && ca.y < 0.0) ? -1. : 1.0;
    return s * sqrt(fmin(dot_ca, dot_cb));
}
