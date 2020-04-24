#ifndef OCTAHEDRON_H_
#define OCTAHEDRON_H_

#include "vec3.h"

/*
Params are:  
- params[0]: s
-> 1 parameter in total
 */
double sdf_octahedron(Vec3 p, double params[]);
extern const int nr_octahedron_params;

#endif // OCTAHEDRON_H_