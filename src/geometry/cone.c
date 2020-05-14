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
    double qx = sqrt(vec_p[0] * vec_p[0] + vec_p[2] * vec_p[2]);
    double qy = vec_p[1];
    
    double cax = qx - fmin(qx, (qy < 0.) ? params[0] : params[1]);
    double cay = fabs(qy) - params[2];

    double vsubx = params[1] - qx;
    double vsuby = params[2] - qy;

    double coeff = clamp(((vsubx*params[3] + vsuby*params[4]) * params[5]), 0.0, 1.0);
    double tmpx = coeff * params[3];
    double tmpy = coeff * params[4];

    double v_sub2x = qx - params[1];
    double v_sub2y = qy - params[2];

    double cbx = v_sub2x + tmpx;
    double cby = v_sub2y + tmpy;

    double dot_ca = cax*cax + cay*cay;
    double dot_cb = cbx*cbx + cby*cby;

    double s = 1-2*(cbx < 0.0 && cay < 0.0);
    return s * sqrt(fmin(dot_ca, dot_cb));
}
