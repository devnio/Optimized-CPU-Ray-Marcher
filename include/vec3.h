#ifndef VEC_3
#define VEC_3

#include <math.h>
#include <stdlib.h>

typedef struct
{
    double x, y, z;
} Vec3;

Vec3 new_vector(double x, double y, double z);
Vec3* new_vector_p(double x, double y, double z);
void set_vec_from_double(Vec3* v, const double xyz);
void vec_mult(const Vec3 *v1, const Vec3 *v2, Vec3 *res);
void vec_pow_inplace(Vec3 *v1, const double p);
void vec_pow(const Vec3 *v1, const double p, Vec3 *res);
void vec_mult_scalar(const Vec3 *v, const double m, Vec3 *res);
void vec_add(const Vec3 *v1, const Vec3 *v2, Vec3 *res);
void vec_add_scalar(const Vec3 *v, const double m, Vec3* res);
void vec_sub(const Vec3 *v1, const Vec3 *v2, Vec3 *res);
double vec_norm(const Vec3 *v);
void vec_normalize(Vec3 *v);
double vec_dot(const Vec3 *v1, const Vec3 *v2);
void vec_cross(const Vec3 *u, const Vec3 *v, Vec3 *res);

Vec3 vec_reflect(Vec3 v, Vec3 normal);

Vec3 vec_abs(Vec3 v);

Vec3 vec_rotate(Vec3 v, Vec3 k, double theta);

Vec3 vec_max(Vec3 v1, Vec3 v2);

#endif
