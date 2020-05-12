#include "vec3.h"
#include "utility.h"
#include "config.h"




/*
 *Function: new_vector
 *----------------------------
 *  Returns a new vector pointer pointing 
 *  to malloced memory on heap
 */
double* new_vector(double x, double y, double z)
{
    double *v = malloc(NR_VEC_ELEMENTS  *sizeof(double)); 

    v[0] = x;
    v[1] = y;
    v[2] = z;
    return v;
}

void delete_vector(double *v) {
    free(v);
}

/*
 *Function: set_zero
 *----------------------------
 *  Sets given vector components to zero
 */
void set_zero(double *vec)
{
    vec[0] = 0.0;
    vec[1] = 0.0;
    vec[2] = 0.0;
}

/*
 *Function: set_vec_from_double
 *----------------------------
 *  Sets all components of given vector to a given double
 *
 *  v: vector to be set to xyz value
 *  xyz: double value
 *
 */
void set_vec_from_double(double *v, const double xyz)
{
    v[0] = xyz;
    v[1] = xyz;
    v[2] = xyz;
}

/*
 *Function: vec_mult
 *----------------------------
 *  Vector mult of v1 and v2 and result in res
 *
 *  v1: multiplicannd 1
 *  v2: multiplicand 2
 *  res: result vector
 *
 */
void vec_mult(const double *v1, const double *v2, double *res)
{
    res[0] = v1[0]  *v2[0];
    res[1] = v1[1]  *v2[1];
    res[2] = v1[2]  *v2[2];
}

/*
 *Function: vec_pow_inplace
 *----------------------------
 *  Replaces v1 with its power of p
 *
 *  v1: vector to raise to power
 *  p: power value
 *
 *  returns: vector elementwise power of p
 */
void vec_pow_inplace(double *v1, const double p)
{
    v1[0] = pow(v1[0], p);
    v1[1] = pow(v1[1], p);
    v1[2] = pow(v1[2], p);
}

/*
 *Function: vec_pow
 *----------------------------
 *  Vector power of v1 saved in res
 *
 *  v1: vector to raise to power p 
 *  p: power value
 *  res: result vector
 *
 */
void vec_pow(const double *v1, const double p, double *res)
{
    res[0] = pow(v1[0], p);
    res[1] = pow(v1[1], p);
    res[2] = pow(v1[2], p);
}

/*
 *Function: vec_mult_scalar
 *----------------------------
 *  Vector multiplication with a scalar
 *
 *  v: vector to be multiplied
 *  m: scalar of value double
 *  res: result of scalar multiplication
 *
 */
void vec_mult_scalar(const double *v, const double m, double *res)
{
    res[0] = v[0]  *m;
    res[1] = v[1]  *m;
    res[2] = v[2]  *m;
}

/*
 *Function: vec_add
 *----------------------------
 *  Performs vector addition between two vectors
 *
 *  v1: summand 1
 *  v2: summand 2
 *  res: result vector
 *
 */
void vec_add(const double *v1, const double *v2, double *res)
{
    res[0] = v1[0] + v2[0];
    res[1] = v1[1] + v2[1];
    res[2] = v1[2] + v2[2];
}

/*
 *Function: vec_add_scalar
 *----------------------------
 *  Adds a scalar value to each element of the vector
 *
 *  v: vector summand 1
 *  m: scalar summand 2
 *
 */
void vec_add_scalar(const double *v, const double m, double *res)
{
    res[0] = v[0] + m;
    res[1] = v[1] + m;
    res[2] = v[2] + m;
}

/*
 *Function: vec_sub
 *----------------------------
 *  Computes vector subtraction
 *
 *  v1: of type double *
 *  v2: of type double*
 *
 *  returns: new vector with subtraction between v1 and v2 
 */
void vec_sub(const double *v1, const double *v2, double *res)
{
    res[0] = v1[0] - v2[0];
    res[1] = v1[1] - v2[1];
    res[2] = v1[2] - v2[2];
}

/*
 *Function: vec_norm
 *----------------------------
 *  Computes vector norm.
 *
 *  v: vector of which the norm is computed
 *
 *  returns: scalar norm of the given vector
 */
double vec_norm(const double *v)
{
    return sqrt(v[0]  *v[0] + v[1]  *v[1] + v[2]  *v[2]);
}

/*
 *Function: vec_normalize
 *----------------------------
 *  Normalizes the given vector in-place
 *
 *  v: Vector to be normalized
 */
void vec_normalize(double *v)
{
    double norm = vec_norm(v);
    if (norm == 0.0 || norm == NAN)
        return;
    vec_mult_scalar(v, 1 / norm, v);
}

/*
 *Function: vec_dot
 *----------------------------
 *  Computes the dot product between the two vectors
 *
 *  v1: first vector
 *  v2: second vector
 *
 *  returns: resulting scalar value of the dot operation between v1 and v2
 */
double vec_dot(const double *v1, const double *v2)
{
    return v1[0]  *v2[0] + v1[1]  *v2[1] + v1[2]  *v2[2];
}

/*
 *Function: vec_cross
 *----------------------------
 *  Computes the cross product between vectors u and v
 *
 *  u: first vector 
 *  v: second vector
 *  res: result vector of the operation
 */
void vec_cross(const double *u, const double *v, double *res)
{
    res[0] = u[1]  *v[2] - v[1]  *u[2];
    res[1] = u[2]  *v[0] - v[2]  *u[0];
    res[2] = u[0]  *v[1] - v[0]  *u[1];
}

/*
 *Function: vec_reflect
 *----------------------------
 *  Computes the reflection of the vector around a normal
 *
 *  v: input vector
 *  normal: normal vector
 *  res: result vector of the operation
 */
void vec_reflect(const double *v, const double *normal, double *res)
{
    vec_mult_scalar(normal, vec_dot(v, normal), res);
    vec_mult_scalar(res, 2, res);
    vec_sub(v, res, res);
}

/*
 *Function: vec_abs
 *----------------------------
 *  Computes component-wise absolute value
 *
 *  v: computes in-place abs value component-wise
 *
 */
void vec_abs(double *v)
{
    v[0] = fabs(v[0]);
    v[1] = fabs(v[1]);
    v[2] = fabs(v[2]);
}


/*
 *Function: vec_max
 *----------------------------
 *  Elementwise max of 2 vectors
 *
 *  v1: of type double *
 *  v2: of type double *
 *
 *  returns: absolute valued vector
 */
void vec_max(const double *v1, const double *v2, double *res)
{
    res[0] = max(v1[0], v2[0]);
    res[1] = max(v1[1], v2[1]);
    res[2] = max(v1[2], v2[2]);
}


/*
 *Function: vec_rotate
 *----------------------------
 *  Rotates the vector around a given axis k
 *
 *  v: of type double *
 *  k: of type double *
 *  theta: in radians
 *
 *  returns: a new vecotr rotated around an axis by a given angle
 */
void vec_rotate(double *v, double *k, double theta, double *res)
{
    //based on Euler rodrigues formula
    double cosTheta = cos(theta);
    double *first;
    vec_mult_scalar(v, cosTheta, &first);
    double *second;
    double *tmp; 
    vec_cross(k, v, &tmp);
    vec_mult_scalar(&tmp, sin(theta), &second);
    double *third;
    vec_mult_scalar(k, vec_dot(k, v)  *(1 - cosTheta), &third);

    vec_add(&second, &third, res);
    vec_add(&first, res, res);
}


/*
 *Function: vec_mode
 *----------------------------
 *  modulo of vector 1 by vector 2 element wise
 */
double* vec_mod(double *v1, double *v2){
    return new_vector(mod(v1[0], v2[0]), mod(v1[1], v2[1]), mod(v1[2], v2[2]));
}
