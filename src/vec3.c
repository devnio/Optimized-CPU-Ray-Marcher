#include "vec3.h"

Vec3 new_vector(double x, double y, double z)
{
    Vec3 v;
    v.x = x; v.y = y; v.z = z;
    return v;
}

Vec3 vec_mult(Vec3 v1, Vec3 v2)
{
    return new_vector(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

Vec3 vec_mult1(Vec3 v, double m)
{
    return new_vector(v.x * m, v.y * m, v.z * m);
}

Vec3 vec_add(Vec3 v1, Vec3 v2)
{
    return new_vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3 vec_add1(Vec3 v, double m)
{
    return new_vector(v.x + m, v.y + m, v.z + m);
}

Vec3 vec_sub(Vec3 v1, Vec3 v2)
{
    return new_vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

double vec_norm(Vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec_normalized(Vec3 v)
{
    double norm = vec_norm(v);
    return vec_mult1(v, 1/norm);
}

double vec_dot(Vec3 v1, Vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 vec_cross(Vec3 u, Vec3 v){
    Vec3 result = {0,0,0};
    result.x = u.y*v.z-v.y*u.z;
    result.y = u.z*v.x-v.z*u.x;
    result.z = u.x*v.y-v.x*u.y;
    return result;
}

Vec3 vec_reflect(Vec3 v, Vec3 normal)
{
    return vec_sub(v, vec_mult1(vec_mult1(normal, vec_dot(v, normal)), 2));
}
