#ifndef SPHERE_H_
#define SPHERE_H_

#include "vec3.h"
/*
Params for plane are:  
- params[0]: radius
-> 1 parameters in total
 */
// double sdf_sphere(const double vec_p[NR_VEC_ELEMENTS], double params[]);
void sdf_sphere(SIMD_VEC* simd_vec_p, double params[], SIMD_MMD* simd_mmd_dists);
extern const int nr_sphere_params;

#endif