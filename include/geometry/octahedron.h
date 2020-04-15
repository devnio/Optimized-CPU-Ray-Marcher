#ifndef OCTAHEDRON_H_
#define OCTAHEDRON_H_

#ifndef VEC_3
 #include "vec3.h"
#endif
#include "material.h"


typedef struct
{
    Vec3 c;
    double s;
    Material mat;
} Octahedron;

double sdf_octahedron(Vec3 p, Octahedron octahedron);


#endif // OCTAHEDRON_H_