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
void new_vector(Vec3* v, double x, double y, double z)
{
    v->x = x;
    v->y = y;
    v->z = z;
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
void set_vec_from_double(Vec3* v, const double xyz)
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
void vec_mult(const Vec3 *v1, const Vec3 *v2, Vec3 *res)
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
void vec_pow_inplace(Vec3 *v1, const double p)
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
void vec_pow(const Vec3 *v1, const double p, Vec3 *res)
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
void vec_mult_scalar(const Vec3 *v, const double m, Vec3 *res)
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
void vec_add(const Vec3 *v1, const Vec3 *v2, Vec3 *res)
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
void vec_add_scalar(const Vec3 *v, const double m, Vec3* res)
{
    res->x = v->x + m;
    res->y = v->y + m;
    res->z = v->z + m;
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
void vec_sub(const Vec3 *v1, const Vec3 *v2, Vec3 *res)
{
    res->x = v1->x - v2->x;
    res->y = v1->y - v2->y;
    res->z = v1->z - v2->z;
}

/*
 * Function: vec_norm
 * ----------------------------
 *   Computes vector norm.
 *
 *   v: vector of which the norm is computed
 *
 *   returns: scalar norm of the given vector
 */
double vec_norm(const Vec3 *v)
{
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}


double vec_norm_squared(const Vec3 *v)
{
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

/*
 * Function: vec_normalize
 * ----------------------------
 *   Normalizes the given vector in-place
 *
 *   v: Vector to be normalized
 */
void vec_normalize(Vec3 *v)
{
    double norm = vec_norm(v);
    if (norm == 0.0 || norm == NAN)
        return;
    vec_mult_scalar(v, 1 / norm, v);
}

/*
 * Function: vec_dot
 * ----------------------------
 *   Computes the dot product between the two vectors
 *
 *   v1: first vector
 *   v2: second vector
 *
 *   returns: resulting scalar value of the dot operation between v1 and v2
 */
double vec_dot(const Vec3 *v1, const Vec3 *v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

/*
 * Function: vec_cross
 * ----------------------------
 *   Computes the cross product between vectors u and v
 *
 *   u: first vector 
 *   v: second vector
 *   res: result vector of the operation
 */
void vec_cross(const Vec3 *u, const Vec3 *v, Vec3 *res)
{
    res->x = u->y * v->z - v->y * u->z;
    res->y = u->z * v->x - v->z * u->x;
    res->z = u->x * v->y - v->x * u->y;
}

/*
 * Function: vec_reflect
 * ----------------------------
 *   Computes the reflection of the vector around a normal
 * 
 *   v: input vector
 *   normal: normal vector
 *   res: result vector of the operation
 */
void vec_reflect(const Vec3 *v, const Vec3 *normal, Vec3 *res)
{
    vec_mult_scalar(normal, vec_dot(v, normal), res);
    vec_mult_scalar(res, 2, res);
    vec_sub(v, res, res);
}

/*
 * Function: vec_abs
 * ----------------------------
 *   Computes component-wise absolute value
 *
 *   v: computes in-place abs value component-wise
 * 
 */
void vec_abs(Vec3 *v)
{
    v->x = fabs(v->x);
    v->y = fabs(v->y);
    v->z = fabs(v->z);
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
void vec_max(const Vec3 *v1, const Vec3 *v2, Vec3 *res)
{
    res->x = max(v1->x, v2->x);
    res->y = max(v1->y, v2->y);
    res->z = max(v1->z, v2->z);
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
void vec_rotate(Vec3 *v, Vec3 *k, double theta, Vec3 *res)
{
    //based on Euler rodrigues formula
    double cosTheta = cos(theta);
    Vec3 first;
    vec_mult_scalar(v, cosTheta, &first);
    Vec3 second;
    Vec3 tmp; 
    vec_cross(k, v, &tmp);
    vec_mult_scalar(&tmp, sin(theta), &second);
    Vec3 third;
    vec_mult_scalar(k, vec_dot(k, v) * (1 - cosTheta), &third);

    vec_add(&second, &third, res);
    vec_add(&first, res, res);
}



/*
 * Function: vec_mode
 * ----------------------------
 *   modulo of vector 1 by vector 2 element wise
 */
void vec_mod(Vec3 *v1, Vec3 *v2, Vec3 *res){
    res->x = mod(v1->x, v2->x);
    res->y = mod(v1->y, v2->y);
    res->z = mod(v1->z, v2->z);
}
