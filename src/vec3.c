#include "vec3.h"
#include "utility.h"

/*
 * Function: new_vector
 * ----------------------------
 *   Returns a new vector
 *
 *   x: double value 
 *   y: double value
 *   z: double value
 *
 *   returns: a new vector with the specified values
 */
Vec3 new_vector(double x, double y, double z)
{
    Vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

/*
 * Function: new_vector
 * ----------------------------
 *   Returns a new vector
 *
 *   x: double value 
 *   y: double value
 *   z: double value
 *
 *   returns: a new vector with the specified values
 */
Vec3 new_vector_one(double xyz)
{
    Vec3 v;
    v.x = xyz;
    v.y = xyz;
    v.z = xyz;
    return v;
}

/*
 * Function: vec_mult
 * ----------------------------
 *   Returns a new vector
 *
 *   v1: of type Vec3 
 *   v2: of type Vec3
 *
 *   returns: returns a new vector that is the vector product between v1 and v2
 */
Vec3 vec_mult(Vec3 v1, Vec3 v2)
{
    return new_vector(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

/*
 * Function: vec_pow
 * ----------------------------
 *   Returns a new vector
 *
 *   v1: of type Vec3 
 *   p: power value
 *
 *   returns: vector elementwise power of p
 */
Vec3 vec_pow(Vec3 v1, double p)
{
    return new_vector(pow(v1.x, p), pow(v1.y, p), pow(v1.z, p));
}

/*
 * Function: vec_mult_scalar
 * ----------------------------
 *   Returns a new vector
 *
 *   v: of type Vec3 
 *   m: of type double
 *
 *   returns: a new vector that is the result of vector v multiplied by scalar m
 */
Vec3 vec_mult_scalar(Vec3 v, double m)
{
    return new_vector(v.x * m, v.y * m, v.z * m);
}

/*
 * Function: vec_add
 * ----------------------------
 *   Performs vector addition between two vectors
 *
 *   v1: of type Vec3 
 *   v2: of type Vec3
 *
 *   returns: returns a new vector including the addition of v1 and v2
 */
Vec3 vec_add(Vec3 v1, Vec3 v2)
{
    return new_vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/*
 * Function: vec_add_scalar
 * ----------------------------
 *   Adds a scalar value to each element of the vector
 *
 *   v: of type Vec3 
 *   m: of type double
 *
 *   returns: a new vector with the specified values
 */
Vec3 vec_add_scalar(Vec3 v, double m)
{
    return new_vector(v.x + m, v.y + m, v.z + m);
}

/*
 * Function: vec_sub
 * ----------------------------
 *   Computes vector subtraction
 *
 *   v1: of type Vec3 
 *   v2: of type Vec3
 *
 *   returns: new vector with subtraction between v1 and v2 
 */
Vec3 vec_sub(Vec3 v1, Vec3 v2)
{
    return new_vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

/*
 * Function: vec_norm
 * ----------------------------
 *   Computes the norm of the vector 
 *
 *   v1: of type Vec3 
 *
 *   returns: norm of the given vector
 */
double vec_norm(Vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/*
 * Function: vec_norm
 * ----------------------------
 *   Normalizes the given vector
 *
 *   v1: of type Vec3 
 *
 *   returns: normalized vector
 */
Vec3 vec_normalized(Vec3 v)
{
    double norm = vec_norm(v);
    if (norm == 0.0 || norm == NAN)
        return v;
    return vec_mult_scalar(v, 1 / norm);
}

/*
 * Function: vec_dot
 * ----------------------------
 *   Computes the dot product between the two vectors
 *
 *   v1: of type Vec3 
 *   v2: of type Vec3 
 *
 *   returns: a new vector that is the dot product between v1 and v2
 */
double vec_dot(Vec3 v1, Vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/*
 * Function: vec_cross
 * ----------------------------
 *   Computes the cross product between v1 and v2
 *
 *   u: of type Vec3 
 *   v: of type Vec3 
 *
 *   returns: new vector that is the cross product between u and v
 */
Vec3 vec_cross(Vec3 u, Vec3 v)
{
    Vec3 result = {0, 0, 0};
    result.x = u.y * v.z - v.y * u.z;
    result.y = u.z * v.x - v.z * u.x;
    result.z = u.x * v.y - v.x * u.y;
    return result;
}

/*
 * Function: vec_reflect
 * ----------------------------
 *   Computes the reflection of the vector around a normal
 * 
 *   v: of type Vec3 
 *   normal: of type Vec3 
 *
 *   returns: a new vector that is the reflection of v around the normal
 */
Vec3 vec_reflect(Vec3 v, Vec3 normal)
{
    return vec_sub(v, vec_mult_scalar(vec_mult_scalar(normal, vec_dot(v, normal)), 2));
}

/*
 * Function: vec_abs
 * ----------------------------
 *   Computes the absolute value of vector
 *
 *   v1: of type Vec3 
 *
 *   returns: absolute valued vector
 */
Vec3 vec_abs(Vec3 v)
{
    return new_vector(fabs(v.x), fabs(v.y), fabs(v.z));
}


/*
 * Function: vec_max
 * ----------------------------
 *   Elementwise max of 2 vectors
 *
 *   v1: of type Vec3 
 *   v2: of type Vec3 
 *
 *   returns: absolute valued vector
 */
Vec3 vec_max(Vec3 v1, Vec3 v2)
{
    Vec3 q;
    q.x = max(v1.x, v2.x);
    q.y = max(v1.y, v2.y);
    q.z = max(v1.z, v2.z);
    return q;
}
/*
 * Function: vec_rotate
 * ----------------------------
 *   Rotates the vector around a given axis k
 * 
 *   v: of type Vec3 
 *   k: of type Vec3 
 *   theta: in radians
 *
 *   returns: a new vecotr rotated around an axis by a given angle
 */
Vec3 vec_rotate(Vec3 v, Vec3 k, double theta)
{
    //based on Euler rodrigues formula
    double cosTheta = cos(theta);
    Vec3 first = vec_mult_scalar(v, cosTheta);
    Vec3 second = vec_mult_scalar(vec_cross(k, v), sin(theta));
    Vec3 third = vec_mult_scalar(k, vec_dot(k, v) * (1 - cosTheta));
    return vec_add(first, vec_add(second, third));
}



/*
 * Function: vec_mode
 * ----------------------------
 *   modulo of vector 1 by vector 2 element wise
 */
Vec3 vec_mod(Vec3 v1, Vec3 v2){
    return new_vector(mod(v1.x, v2.x), mod(v1.y, v2.y), mod(v1.z, v2.z));
}
