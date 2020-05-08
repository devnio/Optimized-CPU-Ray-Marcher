#include "vec3.h"
#include "utility.h"

/*
 * Function: new_vector
 * ----------------------------
 *   Returns a new vector allocated on stack
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
 *   Returns a new vector pointer pointing 
 *   to malloced memory on heap
 */
Vec3* new_vector_p(double x, double y, double z)
{
    Vec3* v = malloc(sizeof(Vec3));

    v->x = x;
    v->y = y;
    v->z = z;
    return v;
}

/*
 * Function: set_vec_from_double
 * ----------------------------
 *   Sets all components of given vector to a given double
 *
 *   v: vector to be set to xyz value
 *   xyz: double value
 * 
 */
void set_vec_from_double(Vec3* v, double xyz)
{
    v->x = xyz;
    v->y = xyz;
    v->z = xyz;
}

/*
 * Function: vec_mult
 * ----------------------------
 *   Vector mult of v1 and v2 and result in res
 *
 *   v1: multiplicannd 1
 *   v2: multiplicand 2
 *   res: result vector
 * 
 */
void vec_mult(Vec3 *v1, Vec3 *v2, Vec3 *res)
{
    res->x = v1->x * v2->x;
    res->y = v1->y * v2->y;
    res->z = v1->z * v2->z;
}

/*
 * Function: vec_pow_inplace
 * ----------------------------
 *   Replaces v1 with its power of p
 *
 *   v1: vector to raise to power
 *   p: power value
 *
 *   returns: vector elementwise power of p
 */
void vec_pow_inplace(Vec3 *v1, double p)
{
    v1->x = pow(v1->x, p);
    v1->y = pow(v1->y, p);
    v1->z = pow(v1->z, p);
}

/*
 * Function: vec_pow
 * ----------------------------
 *   Vector power of v1 saved in res
 *
 *   v1: vector to raise to power p 
 *   p: power value
 *   res: result vector
 *
 */
void vec_pow(Vec3 *v1, double p, Vec3 *res)
{
    res->x = pow(v1->x, p);
    res->y = pow(v1->y, p);
    res->z = pow(v1->z, p);
}

/*
 * Function: vec_mult_scalar
 * ----------------------------
 *   Vector multiplication with a scalar
 *
 *   v: vector to be multiplied
 *   m: scalar of value double
 *   res: result of scalar multiplication
 * 
 */
void vec_mult_scalar(Vec3 *v, double m, Vec3 *res)
{
    res->x = v->x * m;
    res->y = v->y * m;
    res->z = v->z * m;
}

/*
 * Function: vec_add
 * ----------------------------
 *   Performs vector addition between two vectors
 *
 *   v1: summand 1
 *   v2: summand 2
 *   res: result vector
 * 
 */
void vec_add(Vec3 *v1, Vec3 *v2, Vec3 *res)
{
    res->x = v1->x + v2->x;
    res->y = v1->y + v2->y;
    res->z = v1->z + v2->z;
}

/*
 * Function: vec_add_scalar
 * ----------------------------
 *   Adds a scalar value to each element of the vector
 *
 *   v: vector summand 1
 *   m: scalar summand 2
 *
 */
void vec_add_scalar(Vec3 *v, double m, Vec3* res)
{
    res->x = v->x * m;
    res->y = v->y * m;
    res->z = v->z * m;
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
void vec_sub(Vec3 *v1, Vec3 *v2, Vec3 *res)
{
    res->x = v1->x - v2->x;
    res->y = v1->y - v2->y;
    res->z = v1->z - v2->z;
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
    
    Vec3 tmp;
    vec_mult_scalar(&v, 1 / norm, &tmp);
    return tmp;
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
    Vec3 tmp;
    vec_mult_scalar(&normal, vec_dot(v, normal), &tmp);
    vec_mult_scalar(&tmp, 2, &tmp);
    vec_sub(&v, &tmp, &tmp);
    return tmp;
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
    Vec3 first;
    vec_mult_scalar(&v, cosTheta, &first);
    Vec3 second;
    Vec3 tmp = vec_cross(k, v);
    vec_mult_scalar(&tmp, sin(theta), &second);
    Vec3 third;
    vec_mult_scalar(&k, vec_dot(k, v) * (1 - cosTheta), &third);

    Vec3 tmp_add;
    vec_add(&second, &third, &tmp_add);
    vec_add(&first, &tmp_add, &tmp_add);
    return tmp_add;
}
