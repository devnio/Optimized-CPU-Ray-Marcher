#ifndef MANDELBULB_H_
#define MANDELBULB_H_


#include "vec3.h"

#include "material.h"

/*
Params are:  
- There are no params, mandelbulb spawned at position zero
 */
double sdf_mandelbulb(Vec3 p, double params[]);

#endif // MANDELBULB_H_