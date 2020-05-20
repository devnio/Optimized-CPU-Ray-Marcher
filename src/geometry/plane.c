#include "geometry/plane.h"

const int nr_plane_params = 4;

/*
Params for plane are:  
- params[0..2]: normal of plane
- params[3]: displacement of plane
-> 4 parameters in total
 */
void sdf_plane(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists)
{
    SIMD_MMS px = SET1_PS(params[0]);
    SIMD_MMS py = SET1_PS(params[1]);
    SIMD_MMS pz = SET1_PS(params[2]);

    SIMD_MMS dot = ADD_PS(ADD_PS(MULT_PS(simd_vec_p->x, px), MULT_PS(simd_vec_p->y, py)), MULT_PS(simd_vec_p->z, pz));
    *simd_mmd_dists = SUB_PS(dot, SET1_PS(params[3])); 
}
