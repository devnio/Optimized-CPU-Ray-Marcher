#ifndef UTILITY_H_
#define UTILITY_H_

#include "lodepng.h"
#include "vec3.h"
#include "geometry/scene.h"
#include "config.h"
#include "simd.h"

float mix(float a, float b, float mix);
float min(float a, float b);
float max(float a, float b);
float mod(float x, float y);
float clamp(float val, float min, float max);
float sign(float val);
void rotate_point_xyz(SIMD_VEC_PS *vec_p, const float* precomp_orient, SIMD_VEC_PS *vec_res);
float to_radians(float degrees);
char* _concat(const char *str1, const char *str2);
void encodeOneStep(const char *filename, const unsigned char *image, unsigned width, unsigned height);
unsigned char* decodeOneStep(const char* filename, unsigned *out_width, unsigned *out_height, unsigned* error);
void save_image_to_disk(Scene *scene, char* dirName);


void simd_clamp(SIMD_MMS* val, SIMD_MMS min, SIMD_MMS max, SIMD_MMS* res);
#endif
