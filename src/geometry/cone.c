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
double sdf_cone(const double vec_p[NR_VEC_ELEMENTS], double params[])
{
    double v__q[NR_VEC_ELEMENTS]; // local vector declaration
    v__q[0] = sqrt(vec_p[0] * vec_p[0] + vec_p[2] * vec_p[2]);
    v__q[1] = vec_p[1];
    v__q[2] = 0.0;

    double v__k1[NR_VEC_ELEMENTS]; // local vector declaration
    v__k1[0] = params[1];
    v__k1[1] = params[2];
    v__k1[2] = 0.0;

    double v__k2[NR_VEC_ELEMENTS]; // local vector declaration
    v__k2[0] = params[1] - params[0];
    v__k2[1] = 2. * params[2];
    v__k2[2] = 0.0;

    double v__ca[NR_VEC_ELEMENTS]; // local vector declaration
    v__ca[0] = v__q[0] - fmin(v__q[0], (v__q[1] < 0.) ? params[0] : params[1]);
    v__ca[1] = fabs(v__q[1]) - params[2];
    v__ca[2] = 0.0;

    double v__tmp[NR_VEC_ELEMENTS], v__sub[NR_VEC_ELEMENTS];
    vec_sub(v__k1, v__q, v__sub);

    vec_mult_scalar(v__k2, clamp(vec_dot(v__sub, v__k2) / vec_dot(v__k2, v__k2), 0.0, 1.0), v__tmp);

    vec_sub(v__q, v__k1, v__sub);
    vec_add(v__sub, v__tmp, v__sub);
    double s = (v__sub[0] < 0.0 && v__ca[1] < 0.0) ? -1. : 1.0;
    return s * sqrt(fmin(vec_dot(v__ca, v__ca), vec_dot(v__sub, v__sub)));
}
