#include "stdio.h"
#include "geometry/sphere.h"
#include "simd.h"

const int nr_sphere_params = 1;

/*
Params for plane are:  
- params[0]: radius
-> 1 parameters in total
 */
void sdf_sphere(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists)
{
    SIMD_MMS simd_mmd_n;
    simd_vec_norm(simd_vec_p, &simd_mmd_n);

    SIMD_MMS simd_mmd_param0 = SET1_PS(params[0]);
    *simd_mmd_dists = SUB_PS(simd_mmd_n, simd_mmd_param0);
}
