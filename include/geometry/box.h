#ifndef BOX_H_
#define BOX_H_

#include "vec3.h"

/*
Params are:  
- params[0..2]: box extents, x, y, z
-> 3 parameters in total
 */
void sdf_box(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists);
extern const int nr_box_params;

#endif // BOX_H_
