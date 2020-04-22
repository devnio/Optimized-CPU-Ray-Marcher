#ifndef PLANE_H_
#define PLANE_H_

#include "material.h"
#include "vec3.h"
#include "transform.h"

double sdf_plane(Vec3 p, Transform transform, double params[]);

#endif // PLANE_H_