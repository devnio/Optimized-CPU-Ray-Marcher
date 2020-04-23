#ifndef UTILITY
#define UTILITY

#ifndef VEC_3
#include "vec3.h"
#endif

#define OUTPUT_PATH "../output/"

double mix(double a, double b, double mix);
double min(double a, double b);
double max(double a, double b);
double clamp(double value, double max, double min);
Vec3 color_add_weighted(Vec3 colA, Vec3 colB, double weightA, double weightB);
Vec3 color_blend(Vec3 colA, Vec3 colB, double weightA);
double clamp(double val, double min, double max);
double sign(double val);
Vec3 vec_max(Vec3 v1, Vec3 v2);
Vec3 rotate_point(Vec3 p, Vec3 angle);
double to_radians(double degrees);
char* _concat(const char *str1, const char *str2);

#endif