#ifndef CONE
#define CONE

#include "vec3.h"

/*
Params are:  
- params[0]: r1
- params[1]: r2
- params[2]: h
-> 3 parameters in total
 */
void sdf_cone(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists);
extern const int nr_cone_params;

#endif