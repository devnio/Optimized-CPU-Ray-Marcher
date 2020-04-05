#ifndef PLANE_H_
#define PLANE_H_

#include "material.h"
#include "vec3.h"

typedef struct {
    Vec3 n;
    double d; 
    Material mat;
} Plane;

double sdf_plane(Vec3 p, Plane plane);

#endif // PLANE_H_