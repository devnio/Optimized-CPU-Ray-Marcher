#ifndef MANDELBULB_H_
#define MANDELBULB_H_


#include "vec3.h"

#include "material.h"

/*
Params are:  
- There are no params, mandelbulb spawned at position zero
 */
void sdf_mandelbulb(const SIMD_VEC* simd_vec_p, double params[], SIMD_MMD* simd_mmd_dists);


#endif // MANDELBULB_H_