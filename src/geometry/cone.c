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
void sdf_cone(const SIMD_VEC* simd_vec_p, double params[], SIMD_MMD* simd_mmd_dists)
{
    /*SIMD_MMD qx = SQRT_PD(ADD_PD(MULT_PD(simd_vec_p->x, simd_vec_p->x), MULT_PD(simd_vec_p->z, simd_vec_p->z)));
    SIMD_MMD qy = simd_vec_p->y;
    
    SIMD_MMD cax = qx - fmin(qx, (qy < 0.) ? params[0] : params[1]);
    SIMD_MMD cay = fabs(qy) - params[2];

    SIMD_MMD vsubx = params[1] - qx;
    SIMD_MMD vsuby = params[2] - qy;

    SIMD_MMD coeff = clamp(((vsubx*params[3] + vsuby*params[4]) * params[5]), 0.0, 1.0);
    SIMD_MMD tmpx = coeff * params[3];
    SIMD_MMD tmpy = coeff * params[4];

    SIMD_MMD v_sub2x = qx - params[1];
    SIMD_MMD v_sub2y = qy - params[2];

    SIMD_MMD cbx = v_sub2x + tmpx;
    SIMD_MMD cby = v_sub2y + tmpy;

    SIMD_MMD dot_ca = cax*cax + cay*cay;
    SIMD_MMD dot_cb = cbx*cbx + cby*cby;

    SIMD_MMD s = 1-2*(cbx < 0.0 && cay < 0.0);
    *simd_mmd_dists =  s * sqrt(fmin(dot_ca, dot_cb));*/
}
