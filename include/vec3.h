#ifndef VEC_3
#define VEC_3

#include <math.h>
#include <stdlib.h>

#define NR_VEC_ELEMENTS 3

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
double vec_dot(const double *v1, const double *v2);


double *vec_mod(double *v1, double *v2);

#endif
