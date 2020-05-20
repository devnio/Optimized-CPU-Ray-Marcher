#ifndef PLANE_H_
#define PLANE_H_

#include "vec3.h"
#include "geometry/transform.h"

/*
Params for plane are:  
- params[0..2]: normal of plane
- params[3]: displacement of plane
-> 4 parameters in total
 */
void sdf_plane(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists);


/*
For variables use extern keyword so that it only gets declared (NOT yet initialized).
This will be then defined in the .c file. 
Without extern multiple definition exception rises.
*/
extern const int nr_plane_params;

#endif // PLANE_H_