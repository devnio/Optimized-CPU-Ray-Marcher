#ifndef VEC_3
#define VEC_3

#include <math.h>
#include <stdlib.h>
#include "simd.h"

#define NR_VEC_ELEMENTS 3

void debug_simd_vec(const SIMD_VEC* simd_vec);
void debug_simd_mmd(const SIMD_MMD* simd_mmd);

void simd_vec_norm(const SIMD_VEC* simd_vec, SIMD_MMD* simd_mmd_out);
void simd_vec_sub(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res);
void simd_vec_add(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res);
void simd_vec_add_scalar(const SIMD_VEC* simd_vec0, const SIMD_MMD* p, SIMD_VEC* simd_vec_res);
void simd_vec_mult(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res);
void simd_vec_mult_scalar(const SIMD_VEC* simd_vec, SIMD_MMD* p, SIMD_VEC* simd_vec_res);
void simd_vec_norm_squared(const SIMD_VEC* simd_vec, SIMD_MMD* simd_mmd_out);
void simd_vec_normalize(const SIMD_VEC* simd_vec0, SIMD_VEC* simd_vec0_normalized);
void simd_vec_dot(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_MMD* simd_vec_res);
void simd_vec_pow(const SIMD_VEC* simd_vec, SIMD_MMD* p, SIMD_VEC* simd_vec_res);
void simd_vec_cross(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res);
void simd_vec_abs(SIMD_VEC* simd_vec0);
void simd_vec_reflect(const SIMD_VEC* v, const SIMD_VEC* normal, SIMD_VEC* res);
void simd_vec_max(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res);
void simd_vec_rotate(const SIMD_VEC* simd_vec0, const SIMD_VEC* k, SIMD_MMD* theta, SIMD_VEC* simd_vec_res);
void simd_vec_mod(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res);
void simd_mmd_log_func(const SIMD_MMD* simd_mmd0, SIMD_MMD* out_mmd);
// void simd_vec_dot(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_MMD* simd_mmd_out); // TODO remove
// void simd_vec_reflect(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec_N, SIMD_VEC* simd_reflected_vec_out); // TODO remove
void simd_mmd_pow_func(const SIMD_MMD* simd_mmd0, const SIMD_MMD* simd_mmd_pow0, SIMD_MMD* out_mmd);

void create_vec_x(double *v0, double *v1, double *v2, double *v3, double *res);
void create_vec_y(double *v0, double *v1, double *v2, double *v3, double *res);
void create_vec_z(double *v0, double *v1, double *v2, double *v3, double *res);

void simd_set_zero(SIMD_VEC* simd_vec0);
void simd_set_vec_from_double(SIMD_VEC* simd_vec0, const SIMD_MMD* xyz);

double *new_vector(double x, double y, double z);
void delete_vector(double *v);

void set_zero(double *vec);
void vec_add(const double *v1, const double *v2, double *res);
void vec_add_scalar(const double *v, const double m, double *res);
void vec_sub(const double *v1, const double *v2, double *res);
void set_vec_from_double(double *v, const double xyz);
void vec_mult(const double *v1, const double *v2, double *res);
void vec_mult_scalar(const double *v, const double m, double *res);
void vec_pow_inplace(double *v1, const double p);
void vec_pow(const double *v1, const double p, double *res);
void vec_cross(const double *u, const double *v, double *res);
void vec_reflect(const double *v, const double *normal, double *res);
void vec_rotate(double *v, double *k, double theta, double *res);
void vec_max(const double *v1, const double *v2, double *res);
void vec_abs(double *v);
void vec_normalize(double *v);
double vec_norm(const double *v);
double vec_norm_squared(const double *v);
double vec_dot(const double *v1, const double *v2);


void vec_mod(double *v1, double *v2, double *res);

#endif
