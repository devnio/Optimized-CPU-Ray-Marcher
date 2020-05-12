#ifndef BOX_H_
#define BOX_H_

#include "vec3.h"

/*
Params are:  
- params[0..2]: box extents, x, y, z
-> 3 parameters in total
 */
double sdf_box(const double vec_p[NR_VEC_ELEMENTS], double params[]);
extern const int nr_box_params;

#endif // BOX_H_
