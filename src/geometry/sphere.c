#include "stdio.h"
#include "geometry/sphere.h"
#include "simd.h"

const int nr_sphere_params = 1;

/*
Params for plane are:  
- params[0]: radius
-> 1 parameters in total
 */
void sdf_sphere(SIMD_VEC* simd_vec_p, double params[], SIMD_MMD* simd_mmd_dists)
{
    SIMD_MMD simd_mmd_n;
    simd_vec_norm(simd_vec_p, &simd_mmd_n);

    SIMD_MMD simd_mmd_param0 = SET1_PD(params[0]);
    *simd_mmd_dists = SUB_PD(simd_mmd_n, simd_mmd_param0);
}
