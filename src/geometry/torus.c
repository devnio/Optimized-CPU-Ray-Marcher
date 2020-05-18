#include "geometry/torus.h"
#include "simd.h"

const int nr_torus_params = 2;

/*
Params are:  
- params[0]: r1 squared
- params[1]: r2
-> 2 parameter in total
 */
// double sdf_torus(const double vec_p[NR_VEC_ELEMENTS], double params[])
// {
//     double l = vec_p[0] * vec_p[0] + vec_p[2] * vec_p[2];
//     double sqrtL = sqrt(l);
//     return sqrt(l + vec_p[1]*vec_p[1] -2*sqrtL + params[0]) - params[1];
// }

void sdf_torus(const SIMD_VEC* simd_vec, double _params[], SIMD_MMD* res)
{
    SIMD_MMD params[2];
    params[0] = SET1_PD(_params[0]);
    params[1] = SET1_PD(_params[1]);

    SIMD_MMD x2 = MULT_PD(simd_vec->x,simd_vec->x);
    SIMD_MMD l = FMADD_PD(simd_vec->z,simd_vec->z,x2);
    SIMD_MMD sqrtL = SQRT_PD(l);

    SIMD_MMD pres = FMADD_PD(simd_vec->y,simd_vec->y,l);
    SIMD_MMD m2 = SET1_PD(-2.);
    SIMD_MMD pres2 = FMADD_PD(m2, sqrtL, params[0]);

    SIMD_MMD pres3 = SQRT_PD(ADD_PD(pres,pres2));
    *res = SUB_PD(pres3, params[1]);

}

// void simd_sdf_torus(const SIMD_VEC* simd_vec, SIMD_MMD* params, SIMD_MMD* res)
// {
//         SIMD_MMD x2 = MULT_PD(simd_vec->x,simd_vec->x);
//         SIMD_MMD l = FMADD_PD(simd_vec->z,simd_vec->z,x2);
//         SIMD_MMD sqrtL = SQRT_PD(l);

//         SIMD_MMD pres = FMADD_PD(simd_vec->y,simd_vec->y,l);
//         SIMD_MMD m2 = SET1_PD(-2.);
//         SIMD_MMD pres2 = FMADD_PD(m2, sqrtL, params[0]);

//         SIMD_MMD pres3 = SQRT_PD(ADD_PD(pres,pres2));
//         *res = SUB_PD(pres3, params[1]);

// }