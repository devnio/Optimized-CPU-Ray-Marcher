#include <stdio.h>
#include <stdalign.h>

#include "vec3.h"
#include "utility.h"
#include "config.h"
#include "simd.h"

/* Some function for SIMD. */ // TODO: change code so that this is not needed.
void debug_simd_vec(const SIMD_VEC* simd_vec)
{
    alignas(32) double x[NR_SIMD_VEC_ELEMS];
    alignas(32) double y[NR_SIMD_VEC_ELEMS];
    alignas(32) double z[NR_SIMD_VEC_ELEMS];
    STORE_PD(x, simd_vec->x);
    STORE_PD(y, simd_vec->y);
    STORE_PD(z, simd_vec->z);
    printf("\n\n%f, %f, %f, %f \n", x[0], x[1], x[2], x[3]);
    printf("%f, %f, %f, %f \n", y[0], y[1], y[2], y[3]);
    printf("%f, %f, %f, %f \n", z[0], z[1], z[2], z[3]);
    fflush(stdout);
}

void debug_simd_mmd(const SIMD_MMD* simd_mmd)
{
    alignas(32) double v[NR_SIMD_VEC_ELEMS];
    STORE_PD(v, *simd_mmd);
    printf("\nDEBUG SIMD_MMD\n%f, %f, %f, %f\n", v[0], v[1], v[2], v[3]);
}

void create_vec_x(double *v0, double *v1, double *v2, double *v3, double *res)
{
    res[0] = v0[0];
    res[1] = v1[0];
    res[2] = v2[0];
    res[3] = v3[0];
}
void create_vec_y(double *v0, double *v1, double *v2, double *v3, double *res)
{
    res[0] = v0[1];
    res[1] = v1[1];
    res[2] = v2[1];
    res[3] = v3[1];
}
void create_vec_z(double *v0, double *v1, double *v2, double *v3, double *res)
{
    res[0] = v0[2];
    res[1] = v1[2];
    res[2] = v2[2];
    res[3] = v3[2];
}
void simd_set_zero(SIMD_VEC* simd_vec0){
    simd_vec0->x = _mm256_setzero_pd();
    simd_vec0->y = _mm256_setzero_pd();
    simd_vec0->z = _mm256_setzero_pd();
}
void simd_set_vec_from_double(SIMD_VEC* simd_vec0, const SIMD_MMD* xyz){
    simd_vec0->x = *xyz;
    simd_vec0->y = *xyz;
    simd_vec0->z = *xyz;
}
void simd_vec_mult(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res)
{
    simd_vec_res->x = MULT_PD(simd_vec0->x, simd_vec1->x);
    simd_vec_res->y = MULT_PD(simd_vec0->y, simd_vec1->y);
    simd_vec_res->z = MULT_PD(simd_vec0->z, simd_vec1->z);
}

void simd_vec_add(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res)
{
    simd_vec_res->x = ADD_PD(simd_vec0->x, simd_vec1->x);
    simd_vec_res->y = ADD_PD(simd_vec0->y, simd_vec1->y);
    simd_vec_res->z = ADD_PD(simd_vec0->z, simd_vec1->z);
}
void simd_vec_add_scalar(const SIMD_VEC* simd_vec0, const SIMD_MMD* p, SIMD_VEC* simd_vec_res)
{
    simd_vec_res->x = ADD_PD(simd_vec0->x, *p);
    simd_vec_res->y = ADD_PD(simd_vec0->y, *p);
    simd_vec_res->z = ADD_PD(simd_vec0->z, *p);
}

void simd_vec_sub(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res)
{
    simd_vec_res->x = SUB_PD(simd_vec0->x, simd_vec1->x);
    simd_vec_res->y = SUB_PD(simd_vec0->y, simd_vec1->y);
    simd_vec_res->z = SUB_PD(simd_vec0->z, simd_vec1->z);
}
void simd_vec_normalize(const SIMD_VEC* simd_vec0, SIMD_VEC* simd_vec0_normalized)
{
    SIMD_MMD norm;
    simd_vec_norm(simd_vec0, &norm);
    SIMD_MMD inv_norm = DIV_PD(SET1_PD(1.0), norm);

    simd_vec0_normalized->x = MULT_PD(simd_vec0->x, inv_norm);
    simd_vec0_normalized->y = MULT_PD(simd_vec0->y, inv_norm);
    simd_vec0_normalized->z = MULT_PD(simd_vec0->z, inv_norm);
}
void simd_vec_norm(const SIMD_VEC* simd_vec, SIMD_MMD* simd_mmd_out)
{
    *simd_mmd_out = SQRT_PD(ADD_PD(ADD_PD(MULT_PD(simd_vec->x, simd_vec->x), MULT_PD(simd_vec->y, simd_vec->y)), MULT_PD(simd_vec->z, simd_vec->z)));
}
void simd_vec_norm_squared(const SIMD_VEC* simd_vec, SIMD_MMD* simd_mmd_out)
{
    *simd_mmd_out = ADD_PD(ADD_PD(MULT_PD(simd_vec->x, simd_vec->x), MULT_PD(simd_vec->y, simd_vec->y)), MULT_PD(simd_vec->z, simd_vec->z));
}
void simd_vec_dot(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_MMD* simd_vec_res) // TODO: use this code if more performant for vec_norm and vec_normalize and vec_norm_squared
{
    SIMD_MMD tmpx  = MULT_PD(simd_vec0->x, simd_vec1->x);
    SIMD_MMD tmpyx = FMADD_PD(simd_vec0->y, simd_vec1->y, tmpx);
    *simd_vec_res =  FMADD_PD(simd_vec0->z, simd_vec1->z, tmpyx);
}
/*void simd_vec_pow(const SIMD_VEC* simd_vec, SIMD_MMD* p, SIMD_VEC* simd_vec_res) // TODO: fix gcc not recognizing pow
{
    simd_vec_res->x = POW_PD(simd_vec->x,*p);
    simd_vec_res->y = POW_PD(simd_vec->y,*p);
    simd_vec_res->z = POW_PD(simd_vec->z,*p);
}*/
void simd_vec_mult_scalar(const SIMD_VEC* simd_vec, SIMD_MMD* p, SIMD_VEC* simd_vec_res)
{
    simd_vec_res->x = MULT_PD(simd_vec->x, *p);
    simd_vec_res->y = MULT_PD(simd_vec->y, *p);
    simd_vec_res->z = MULT_PD(simd_vec->z, *p);
}

void simd_vec_cross(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res)
{
    SIMD_MMD tmpx = MULT_PD(simd_vec0->z, simd_vec1->y);
    SIMD_MMD tmpy = MULT_PD(simd_vec0->x, simd_vec1->z);
    SIMD_MMD tmpz = MULT_PD(simd_vec0->y, simd_vec1->x);

    simd_vec_res->x = FMSUB_PD(simd_vec0->y, simd_vec1->z, tmpx);
    simd_vec_res->y = FMSUB_PD(simd_vec0->z, simd_vec1->x, tmpy);
    simd_vec_res->z = FMSUB_PD(simd_vec0->x, simd_vec1->y , tmpz);
}
void simd_vec_abs(SIMD_VEC* simd_vec0) // TODO: if not fast enough try other method: https://stackoverflow.com/questions/25590602/how-do-i-perform-absolute-value-on-double-using-intrinsics
{
    const __m256d mask = _mm256_castsi256_pd (_mm256_set1_epi64x (0x7FFFFFFFFFFFFFFF));
    simd_vec0->x = AND_PD (mask, simd_vec0->x);
    simd_vec0->y = AND_PD (mask, simd_vec0->y);
    simd_vec0->z = AND_PD (mask, simd_vec0->z);
}
// void simd_vec_reflect(const SIMD_VEC* v, const SIMD_VEC* normal, SIMD_VEC* res)
// {
//     SIMD_MMD tmp;
//     simd_vec_dot(v, normal, &tmp);
//     simd_vec_mult_scalar(normal, &tmp, res);
//     SIMD_MMD scalar = SET1_PD(2.);
//     simd_vec_mult_scalar(res, &scalar, res);
//     simd_vec_sub(v, res, res);

// }
void simd_vec_max(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res)
{
    simd_vec_res->x = MAX_PD(simd_vec0->x, simd_vec1->x);
    simd_vec_res->y = MAX_PD(simd_vec0->y, simd_vec1->y);
    simd_vec_res->z = MAX_PD(simd_vec0->z, simd_vec1->z);
}
/*void simd_vec_rotate(const SIMD_VEC* simd_vec0, const SIMD_VEC* k, SIMD_MMD* theta, SIMD_VEC* simd_vec_res)
{

    SIMD_MMD cosTheta = COS_PD(*theta);
    SIMD_VEC first;
    simd_vec_mult_scalar(simd_vec0, &cosTheta, &first);
    
    SIMD_VEC second;
    SIMD_VEC tmp; 
    simd_vec_cross(k, simd_vec0, &tmp);
    SIMD_MMD sinTheta = SIN_PD(*theta);
    simd_vec_mult_scalar(&tmp, &sinTheta, &second);
    SIMD_VEC third;

    SIMD_MMD ct = SUB_PD(SET1_PD(1.), cosTheta);
    SIMD_MMD pres;
    simd_vec_dot(k,simd_vec0, &pres);
    SIMD_MMD _tmp = MULT_PD(pres, ct); // todo finish vec_dot 
    simd_vec_mult_scalar(k, &_tmp, &third);

    simd_vec_add(&second, &third, simd_vec_res);
    simd_vec_add(&first, simd_vec_res, simd_vec_res);

}*/

void simd_vec_mod(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_VEC* simd_vec_res)
{
    simd_vec_res->x = MULT_PD(simd_vec1->x, _mm256_floor_pd(_mm256_div_pd(simd_vec0->x,simd_vec1->x)));
    simd_vec_res->y = MULT_PD(simd_vec1->y, _mm256_floor_pd(_mm256_div_pd(simd_vec0->y,simd_vec1->y)));
    simd_vec_res->z = MULT_PD(simd_vec1->z, _mm256_floor_pd(_mm256_div_pd(simd_vec0->z,simd_vec1->z)));

    simd_vec_res->x = SUB_PD(simd_vec0->x, simd_vec_res->x);
    simd_vec_res->y = SUB_PD(simd_vec0->y, simd_vec_res->y);
    simd_vec_res->z = SUB_PD(simd_vec0->z, simd_vec_res->z);  

}



// TODO remove
// void simd_vec_dot(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_MMD* simd_mmd_out)
// {
//     *simd_mmd_out = ADD_PD(ADD_PD(MULT_PD(simd_vec0->x, simd_vec1->x), MULT_PD(simd_vec0->y, simd_vec1->y)), MULT_PD(simd_vec0->z, simd_vec1->z));
// }
void simd_vec_reflect(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec_N, SIMD_VEC* simd_reflected_vec_out)
{
    SIMD_MMD dot;
    simd_vec_dot(simd_vec0, simd_vec_N, &dot);

    SIMD_MMD r_x = MULT_PD(dot, simd_vec_N->x);
    SIMD_MMD r_y = MULT_PD(dot, simd_vec_N->y);
    SIMD_MMD r_z = MULT_PD(dot, simd_vec_N->z);
    
    r_x = MULT_PD(r_x, SET1_PD(2.0));
    r_y = MULT_PD(r_y, SET1_PD(2.0));
    r_z = MULT_PD(r_z, SET1_PD(2.0));

    simd_reflected_vec_out->x = SUB_PD(simd_vec0->x, r_x);
    simd_reflected_vec_out->y = SUB_PD(simd_vec0->y, r_y);
    simd_reflected_vec_out->z = SUB_PD(simd_vec0->z, r_z);
}
void simd_mmd_pow_func(const SIMD_MMD* simd_mmd0, const SIMD_MMD* simd_mmd_pow0, SIMD_MMD* out_mmd)
{
    // TODO: do a proper pow using an approximation, e.g. taylor for exp and log and combine
    alignas(32) double v[NR_SIMD_VEC_ELEMS];
    STORE_PD(v, *simd_mmd0);

    alignas(32) double val[NR_SIMD_VEC_ELEMS];
    STORE_PD(val, *simd_mmd_pow0);

    *out_mmd = SET_PD(pow(v[3], val[3]), pow(v[2], val[2]), pow(v[1], val[1]), pow(v[0], val[0]));
}

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
    res[0] = v1[0] * v2[0];
    res[1] = v1[1] * v2[1];
    res[2] = v1[2] * v2[2];
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
    res[0] = v[0] * m;
    res[1] = v[1] * m;
    res[2] = v[2] * m;
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
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}


double vec_norm_squared(const double *v)
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
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
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
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
    res[0] = u[1] * v[2] - v[1] * u[2];
    res[1] = u[2] * v[0] - v[2] * u[0];
    res[2] = u[0] * v[1] - v[0] * u[1];
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
    double first[NR_VEC_ELEMENTS];
    vec_mult_scalar(v, cosTheta, first);
    double second[NR_VEC_ELEMENTS];
    double tmp[NR_VEC_ELEMENTS]; 
    vec_cross(k, v, tmp);
    vec_mult_scalar(tmp, sin(theta), second);
    double third[NR_VEC_ELEMENTS];
    vec_mult_scalar(k, vec_dot(k, v) * (1 - cosTheta), third);

    vec_add(second, third, res);
    vec_add(first, res, res);
}


/*
 *Function: vec_mode
 *----------------------------
 *  modulo of vector 1 by vector 2 element wise
 */
void vec_mod(double *v1, double *v2, double *res){
    res[0] = mod(v1[0], v2[0]);
    res[1] = mod(v1[1], v2[1]);
    res[2] = mod(v1[2], v2[2]);
}
