#ifndef SPHERE_H_
#define SPHERE_H_

#include "vec3.h"
/*
Params for plane are:  
- params[0]: radius
-> 1 parameters in total
 */
// float sdf_sphere(const float vec_p[NR_VEC_ELEMENTS], float params[]);
void sdf_sphere(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists);
extern const int nr_sphere_params;

#endif