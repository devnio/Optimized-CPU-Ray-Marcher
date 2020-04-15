#ifndef BOX_H_
#define BOX_H_

#include "material.h"
#include "vec3.h"
#include "utility.h"

typedef struct {
    Vec3 b;
    Material mat;
} Box;

float sdf_box(Vec3 p, Box box);

#endif // BOX_H_