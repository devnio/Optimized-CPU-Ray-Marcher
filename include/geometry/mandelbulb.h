#ifndef MANDELBULB_H_
#define MANDELBULB_H_


#include "vec3.h"

#include "material.h"

/*
Params are:  
- There are no params, mandelbulb spawned at position zero
 */
double sdf_mandelbulb(const double vec_p[NR_VEC_ELEMENTS], double params[]);

#endif // MANDELBULB_H_