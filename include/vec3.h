#include <math.h>


/* ===============
 * Vector 3 - START
 * =============== */

typedef struct {
    double x, y, z;
} Vec3;

Vec3 new_vector(double x, double y, double z);

Vec3 vec_mult(Vec3 v1, Vec3 v2);

Vec3 vec_mult1(Vec3 v, double m);

Vec3 vec_add(Vec3 v1, Vec3 v2);

Vec3 vec_add1(Vec3 v, double m);

Vec3 vec_sub(Vec3 v1, Vec3 v2);

double vec_norm(Vec3 v);

Vec3 vec_normalized(Vec3 v);

double vec_dot(Vec3 v1, Vec3 v2);

Vec3 vec_cross(Vec3 u, Vec3 v);

Vec3 vec_reflect(Vec3 v, Vec3 normal);