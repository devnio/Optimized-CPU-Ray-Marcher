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
void set_vec_from_double(Vec3* v, double xyz);
void vec_mult(Vec3 *v1, Vec3 *v2, Vec3 *res);
void vec_pow_inplace(Vec3 *v1, double p);
void vec_pow(Vec3 *v1, double p, Vec3 *res);
void vec_mult_scalar(Vec3 *v, double m, Vec3 *res);
void vec_add(Vec3 *v1, Vec3 *v2, Vec3 *res);
void vec_add_scalar(Vec3 *v, double m, Vec3* res);
void vec_sub(Vec3 *v1, Vec3 *v2, Vec3 *res);

double vec_norm(Vec3 v);

Vec3 vec_normalized(Vec3 v);

double vec_dot(Vec3 v1, Vec3 v2);

Vec3 vec_cross(Vec3 u, Vec3 v);

Vec3 vec_reflect(Vec3 v, Vec3 normal);

Vec3 vec_abs(Vec3 v);

Vec3 vec_rotate(Vec3 v, Vec3 k, double theta);

Vec3 vec_max(Vec3 v1, Vec3 v2);

#endif
