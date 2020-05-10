#ifndef UTILITY_H_
#define UTILITY_H_

#include "lodepng.h"
#include "vec3.h"
#include "geometry/scene.h"
#include "config.h"

double mix(double a, double b, double mix);
double min(double a, double b);
double max(double a, double b);
double mod(double x, double y);
double clamp(double val, double min, double max);
double sign(double val);
Vec3 rotate_point_xyz(Vec3 p, const double* precomp_orient);
double to_radians(double degrees);
char* _concat(const char *str1, const char *str2);
void encodeOneStep(const char *filename, const unsigned char *image, unsigned width, unsigned height);
unsigned char* decodeOneStep(const char* filename, unsigned *out_width, unsigned *out_height, unsigned* error);
void save_image_to_disk(Scene *scene, char* dirName);

#endif
