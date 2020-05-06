#ifndef UTILITY_H_
#define UTILITY_H_

#include "lodepng.h"
#include "vec3.h"

#define OUTPUT_PATH "../output"
#define RENDER_OUT "../output/render_out/"

// ANTI ALIASING
#define AA 1

// GAMMA CORRECTION
#define GAMMA_CORR 1

// DEBUG
#define DEBUG_MODE 0


double mix(double a, double b, double mix);
double min(double a, double b);
double max(double a, double b);
double clamp(double value, double max, double min);
Vec3 color_add_weighted(Vec3 colA, Vec3 colB, double weightA, double weightB);
Vec3 color_blend(Vec3 colA, Vec3 colB, double weightA);
double clamp(double val, double min, double max);
double sign(double val);
Vec3 rotate_point_x(Vec3 p, double angle);
Vec3 rotate_point_y(Vec3 p, double angle);
Vec3 rotate_point_z(Vec3 p, double angle);
double to_radians(double degrees);
char* _concat(const char *str1, const char *str2);
void encodeOneStep(const char *filename, const unsigned char *image, unsigned width, unsigned height);

#endif