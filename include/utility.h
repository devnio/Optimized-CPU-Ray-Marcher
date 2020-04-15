#ifndef UTILITY
#define UTILITY

#ifndef VEC_3
#include "vec3.h"
#endif


double mix(double a, double b, double mix);
double min(double a, double b);
double max(double a, double b);
double clamp(double value, double max, double min);
Vec3 color_add_weighted(Vec3 colA, Vec3 colB, double weightA, double weightB);
Vec3 color_blend(Vec3 colA, Vec3 colB, double weightA);

#endif