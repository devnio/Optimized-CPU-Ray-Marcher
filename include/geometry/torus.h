#ifndef TORUS_H_
#define TORUS_H_

#include "vec3.h"

/*
Params are:  
- params[0]: r1
- params[1]: r2
-> 2 parameter in total
 */
void sdf_torus(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists);
extern const int nr_torus_params;

#endif //TORUS_H_