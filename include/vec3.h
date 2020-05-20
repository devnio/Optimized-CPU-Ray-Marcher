#ifndef VEC_3
#define VEC_3

#include <math.h>
#include <stdlib.h>
#include "simd.h"

#define NR_VEC_ELEMENTS 3

void debug_simd_vec(const SIMD_VEC_PS* simd_vec);
void debug_simd_mmd(const SIMD_MMS* simd_mmd);

void simd_vec_norm(const SIMD_VEC_PS* simd_vec, SIMD_MMS* simd_mmd_out);
void simd_vec_sub(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res);
void simd_vec_add(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res);
void simd_vec_add_scalar(const SIMD_VEC_PS* simd_vec0, const SIMD_MMS* p, SIMD_VEC_PS* simd_vec_res);
void simd_vec_mult(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res);
void simd_vec_mult_scalar(const SIMD_VEC_PS* simd_vec, SIMD_MMS* p, SIMD_VEC_PS* simd_vec_res);
void simd_vec_norm_squared(const SIMD_VEC_PS* simd_vec, SIMD_MMS* simd_mmd_out);
void simd_vec_normalize(const SIMD_VEC_PS* simd_vec0, SIMD_VEC_PS* simd_vec0_normalized);
void simd_vec_dot(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_MMS* simd_vec_res);
void simd_vec_pow(const SIMD_VEC_PS* simd_vec, SIMD_MMS* p, SIMD_VEC_PS* simd_vec_res);
void simd_vec_cross(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res);
void simd_vec_abs(SIMD_VEC_PS* simd_vec0);
void simd_vec_reflect(const SIMD_VEC_PS* v, const SIMD_VEC_PS* normal, SIMD_VEC_PS* res);
void simd_vec_max(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res);
void simd_vec_rotate(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* k, SIMD_MMS* theta, SIMD_VEC_PS* simd_vec_res);
void simd_vec_mod(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res);
void simd_mmd_log_func(const SIMD_MMS* simd_mmd0, SIMD_MMS* out_mmd);
// void simd_vec_dot(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_MMD* simd_mmd_out); // TODO remove
// void simd_vec_reflect(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec_N, SIMD_VEC* simd_reflected_vec_out); // TODO remove
void simd_mmd_pow_func(const SIMD_MMS* simd_mmd0, const SIMD_MMS* simd_mmd_pow0, SIMD_MMS* out_mmd);

// void create_vec_x(float *v0, float *v1, float *v2, float *v3, float *res);
// void create_vec_y(float *v0, float *v1, float *v2, float *v3, float *res);
// void create_vec_z(float *v0, float *v1, float *v2, float *v3, float *res);

void simd_set_zero(SIMD_VEC_PS* simd_vec0);
void simd_set_vec_from_float(SIMD_VEC_PS* simd_vec0, const SIMD_MMS* xyz);

float *new_vector(float x, float y, float z);
void delete_vector(float *v);

void set_zero(float *vec);
void vec_add(const float *v1, const float *v2, float *res);
void vec_add_scalar(const float *v, const float m, float *res);
void vec_sub(const float *v1, const float *v2, float *res);
void set_vec_from_float(float *v, const float xyz);
void vec_mult(const float *v1, const float *v2, float *res);
void vec_mult_scalar(const float *v, const float m, float *res);
void vec_pow_inplace(float *v1, const float p);
void vec_pow(const float *v1, const float p, float *res);
void vec_cross(const float *u, const float *v, float *res);
void vec_reflect(const float *v, const float *normal, float *res);
void vec_rotate(float *v, float *k, float theta, float *res);
void vec_max(const float *v1, const float *v2, float *res);
void vec_abs(float *v);
void vec_normalize(float *v);
float vec_norm(const float *v);
float vec_norm_squared(const float *v);
float vec_dot(const float *v1, const float *v2);


void vec_mod(float *v1, float *v2, float *res);

#endif
