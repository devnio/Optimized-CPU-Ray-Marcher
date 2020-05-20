#ifndef MANDELBULB_H_
#define MANDELBULB_H_


#include "vec3.h"

#include "material.h"

/*
Params are:  
- There are no params, mandelbulb spawned at position zero
 */
void sdf_mandelbulb(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists);


#endif // MANDELBULB_H_