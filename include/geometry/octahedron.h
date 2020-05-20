#ifndef OCTAHEDRON_H_
#define OCTAHEDRON_H_

#include "vec3.h"

/*
Params are:  
- params[0]: s
-> 1 parameter in total
 */
//double sdf_octahedron(const double vec_p[NR_VEC_ELEMENTS], double params[]);
void sdf_octahedron(const SIMD_VEC_PS* simd_vec, float params[], SIMD_MMS* dists);

#endif // OCTAHEDRON_H_