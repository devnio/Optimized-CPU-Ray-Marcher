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
double sdf_cone(const double vec_p[NR_VEC_ELEMENTS], double params[]);
extern const int nr_cone_params;

#endif