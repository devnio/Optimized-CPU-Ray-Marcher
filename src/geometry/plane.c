#include "geometry/plane.h"

const int nr_plane_params = 4;

/*
Params for plane are:  
- params[0..2]: normal of plane
- params[3]: displacement of plane
-> 4 parameters in total
 */
void sdf_plane(const SIMD_VEC* simd_vec_p, double params[], SIMD_MMD* simd_mmd_dists)
{
    SIMD_MMD px = SET1_PD(params[0]);
    SIMD_MMD py = SET1_PD(params[1]);
    SIMD_MMD pz = SET1_PD(params[2]);

    SIMD_MMD dot = ADD_PD(ADD_PD(MULT_PD(simd_vec_p->x, px), MULT_PD(simd_vec_p->y, py)), MULT_PD(simd_vec_p->z, pz));
    *simd_mmd_dists = SUB_PD(dot, SET1_PD(params[3])); 
}
