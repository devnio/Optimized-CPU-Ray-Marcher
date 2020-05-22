#include <stdio.h>
#include <stdalign.h>

#include "vec3.h"
#include "utility.h"
#include "config.h"
#include "simd.h"

/* Some function for SIMD. */ // TODO: change code so that this is not needed.
void debug_simd_vec(const SIMD_VEC_PS* simd_vec)
{
    alignas(32) float x[NR_SIMD_VEC_ELEMS];
    alignas(32) float y[NR_SIMD_VEC_ELEMS];
    alignas(32) float z[NR_SIMD_VEC_ELEMS];
    STORE_PS(x, simd_vec->x);
    STORE_PS(y, simd_vec->y);
    STORE_PS(z, simd_vec->z);
    printf("\n\n%f, %f, %f, %f \n", x[0], x[1], x[2], x[3]);
    printf("%f, %f, %f, %f \n", y[0], y[1], y[2], y[3]);
    printf("%f, %f, %f, %f \n", z[0], z[1], z[2], z[3]);
    fflush(stdout);
}

void debug_simd_mmd(const SIMD_MMS* simd_mmd)
{
    alignas(32) float v[NR_SIMD_VEC_ELEMS];
    STORE_PS(v, *simd_mmd);
    printf("\nDEBUG SIMD_MMD\n%f, %f, %f, %f\n", v[0], v[1], v[2], v[3]);
}

// void create_vec_x(float *v0, float *v1, float *v2, float *v3, float *res)
// {
//     res[0] = v0[0];
//     res[1] = v1[0];
//     res[2] = v2[0];
//     res[3] = v3[0];
// }
// void create_vec_y(float *v0, float *v1, float *v2, float *v3, float *res)
// {
//     res[0] = v0[1];
//     res[1] = v1[1];
//     res[2] = v2[1];
//     res[3] = v3[1];
// }
// void create_vec_z(float *v0, float *v1, float *v2, float *v3, float *res)
// {
//     res[0] = v0[2];
//     res[1] = v1[2];
//     res[2] = v2[2];
//     res[3] = v3[2];
// }
void simd_set_zero(SIMD_VEC_PS* simd_vec0){
    simd_vec0->x = SET_ZERO_PS();
    simd_vec0->y = SET_ZERO_PS();
    simd_vec0->z = SET_ZERO_PS();
}
void simd_set_vec_from_float(SIMD_VEC_PS* simd_vec0, const SIMD_MMS* xyz){
    simd_vec0->x = *xyz;
    simd_vec0->y = *xyz;
    simd_vec0->z = *xyz;
}
void simd_vec_mult(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res)
{
    simd_vec_res->x = MULT_PS(simd_vec0->x, simd_vec1->x);
    simd_vec_res->y = MULT_PS(simd_vec0->y, simd_vec1->y);
    simd_vec_res->z = MULT_PS(simd_vec0->z, simd_vec1->z);
}

void simd_vec_add(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res)
{
    simd_vec_res->x = ADD_PS(simd_vec0->x, simd_vec1->x);
    simd_vec_res->y = ADD_PS(simd_vec0->y, simd_vec1->y);
    simd_vec_res->z = ADD_PS(simd_vec0->z, simd_vec1->z);
}
void simd_vec_add_scalar(const SIMD_VEC_PS* simd_vec0, const SIMD_MMS* p, SIMD_VEC_PS* simd_vec_res)
{
    simd_vec_res->x = ADD_PS(simd_vec0->x, *p);
    simd_vec_res->y = ADD_PS(simd_vec0->y, *p);
    simd_vec_res->z = ADD_PS(simd_vec0->z, *p);
}

void simd_vec_sub(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res)
{
    simd_vec_res->x = SUB_PS(simd_vec0->x, simd_vec1->x);
    simd_vec_res->y = SUB_PS(simd_vec0->y, simd_vec1->y);
    simd_vec_res->z = SUB_PS(simd_vec0->z, simd_vec1->z);
}
void simd_vec_normalize(const SIMD_VEC_PS* simd_vec0, SIMD_VEC_PS* simd_vec0_normalized)
{
    SIMD_MMS norm;
    simd_vec_norm(simd_vec0, &norm);
    SIMD_MMS inv_norm = DIV_PS(SET1_PS(1.0), norm);

    simd_vec0_normalized->x = MULT_PS(simd_vec0->x, inv_norm);
    simd_vec0_normalized->y = MULT_PS(simd_vec0->y, inv_norm);
    simd_vec0_normalized->z = MULT_PS(simd_vec0->z, inv_norm);
}
void simd_vec_norm(const SIMD_VEC_PS* simd_vec, SIMD_MMS* simd_mmd_out)
{
    *simd_mmd_out = SQRT_PS_A(ADD_PS(ADD_PS(MULT_PS(simd_vec->x, simd_vec->x), MULT_PS(simd_vec->y, simd_vec->y)), MULT_PS(simd_vec->z, simd_vec->z)));
}
void simd_vec_norm_squared(const SIMD_VEC_PS* simd_vec, SIMD_MMS* simd_mmd_out)
{
    *simd_mmd_out = ADD_PS(ADD_PS(MULT_PS(simd_vec->x, simd_vec->x), MULT_PS(simd_vec->y, simd_vec->y)), MULT_PS(simd_vec->z, simd_vec->z));
}
void simd_vec_dot(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_MMS* simd_vec_res) // TODO: use this code if more performant for vec_norm and vec_normalize and vec_norm_squared
{
    SIMD_MMS tmpx  = MULT_PS(simd_vec0->x, simd_vec1->x);
    SIMD_MMS tmpyx = FMADD_PS(simd_vec0->y, simd_vec1->y, tmpx);
    *simd_vec_res =  FMADD_PS(simd_vec0->z, simd_vec1->z, tmpyx);
}
/*void simd_vec_pow(const SIMD_VEC* simd_vec, SIMD_MMD* p, SIMD_VEC* simd_vec_res) // TODO: fix gcc not recognizing pow
{
    simd_vec_res->x = POW_PD(simd_vec->x,*p);
    simd_vec_res->y = POW_PD(simd_vec->y,*p);
    simd_vec_res->z = POW_PD(simd_vec->z,*p);
}*/
void simd_vec_mult_scalar(const SIMD_VEC_PS* simd_vec, SIMD_MMS* p, SIMD_VEC_PS* simd_vec_res)
{
    simd_vec_res->x = MULT_PS(simd_vec->x, *p);
    simd_vec_res->y = MULT_PS(simd_vec->y, *p);
    simd_vec_res->z = MULT_PS(simd_vec->z, *p);
}

void simd_vec_cross(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res)
{
    SIMD_MMS tmpx = MULT_PS(simd_vec0->z, simd_vec1->y);
    SIMD_MMS tmpy = MULT_PS(simd_vec0->x, simd_vec1->z);
    SIMD_MMS tmpz = MULT_PS(simd_vec0->y, simd_vec1->x);

    simd_vec_res->x = FMSUB_PS(simd_vec0->y, simd_vec1->z, tmpx);
    simd_vec_res->y = FMSUB_PS(simd_vec0->z, simd_vec1->x, tmpy);
    simd_vec_res->z = FMSUB_PS(simd_vec0->x, simd_vec1->y, tmpz);
}
void simd_vec_abs(SIMD_VEC_PS* simd_vec0) // TODO: if not fast enough try other method: https://stackoverflow.com/questions/25590602/how-do-i-perform-absolute-value-on-float-using-intrinsics
{
    const __m256 mask = _mm256_castsi256_ps (_mm256_set1_epi64x (0x7FFFFFFFFFFFFFFF));
    simd_vec0->x = AND_PS (mask, simd_vec0->x);
    simd_vec0->y = AND_PS (mask, simd_vec0->y);
    simd_vec0->z = AND_PS (mask, simd_vec0->z);
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
void simd_vec_max(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res)
{
    simd_vec_res->x = MAX_PS(simd_vec0->x, simd_vec1->x);
    simd_vec_res->y = MAX_PS(simd_vec0->y, simd_vec1->y);
    simd_vec_res->z = MAX_PS(simd_vec0->z, simd_vec1->z);
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

void simd_vec_mod(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec1, SIMD_VEC_PS* simd_vec_res)
{
    simd_vec_res->x = MULT_PS(simd_vec1->x, _mm256_floor_ps(DIV_PS(simd_vec0->x,simd_vec1->x)));
    simd_vec_res->y = MULT_PS(simd_vec1->y, _mm256_floor_ps(DIV_PS(simd_vec0->y,simd_vec1->y)));
    simd_vec_res->z = MULT_PS(simd_vec1->z, _mm256_floor_ps(DIV_PS(simd_vec0->z,simd_vec1->z)));

    simd_vec_res->x = SUB_PS(simd_vec0->x, simd_vec_res->x);
    simd_vec_res->y = SUB_PS(simd_vec0->y, simd_vec_res->y);
    simd_vec_res->z = SUB_PS(simd_vec0->z, simd_vec_res->z);  

}



// TODO remove
// void simd_vec_dot(const SIMD_VEC* simd_vec0, const SIMD_VEC* simd_vec1, SIMD_MMD* simd_mmd_out)
// {
//     *simd_mmd_out = ADD_PD(ADD_PD(MULT_PD(simd_vec0->x, simd_vec1->x), MULT_PD(simd_vec0->y, simd_vec1->y)), MULT_PD(simd_vec0->z, simd_vec1->z));
// }
void simd_vec_reflect(const SIMD_VEC_PS* simd_vec0, const SIMD_VEC_PS* simd_vec_N, SIMD_VEC_PS* simd_reflected_vec_out)
{
    SIMD_MMS dot;
    simd_vec_dot(simd_vec0, simd_vec_N, &dot);

    SIMD_MMS r_x = MULT_PS(dot, simd_vec_N->x);
    SIMD_MMS r_y = MULT_PS(dot, simd_vec_N->y);
    SIMD_MMS r_z = MULT_PS(dot, simd_vec_N->z);
    
    r_x = MULT_PS(r_x, SET1_PS(2.0));
    r_y = MULT_PS(r_y, SET1_PS(2.0));
    r_z = MULT_PS(r_z, SET1_PS(2.0));

    simd_reflected_vec_out->x = SUB_PS(simd_vec0->x, r_x);
    simd_reflected_vec_out->y = SUB_PS(simd_vec0->y, r_y);
    simd_reflected_vec_out->z = SUB_PS(simd_vec0->z, r_z);
}
void simd_mmd_pow_func(const SIMD_MMS* simd_mmd0, const SIMD_MMS* simd_mmd_pow0, SIMD_MMS* out_mmd)
{
    // TODO: do a proper pow using an approximation, e.g. taylor for exp and log and combine
    alignas(32) float v[NR_SIMD_VEC_ELEMS];
    STORE_PS(v, *simd_mmd0);

    alignas(32) float val[NR_SIMD_VEC_ELEMS];
    STORE_PS(val, *simd_mmd_pow0);

    *out_mmd = SET_PS(pow(v[7], val[7]), pow(v[6], val[6]), 
                      pow(v[5], val[5]), pow(v[4], val[4]), 
                      pow(v[3], val[3]), pow(v[2], val[2]), 
                      pow(v[1], val[1]), pow(v[0], val[0]));
}

void simd_mmd_log_func(const SIMD_MMS* simd_mmd0, SIMD_MMS* out_mmd)
{
    // TODO: do a proper pow using an approximation, e.g. taylor for exp and log and combine
    alignas(32) float v[NR_SIMD_VEC_ELEMS];
    STORE_PS(v, *simd_mmd0);

    *out_mmd = SET_PS(log(v[7]), log(v[6]), log(v[5]), log(v[4]), 
                      log(v[3]), log(v[2]), log(v[1]), log(v[0]));
}

/*
 *Function: new_vector
 *----------------------------
 *  Returns a new vector pointer pointing 
 *  to malloced memory on heap
 */
float* new_vector(float x, float y, float z)
{
    float *v = malloc(NR_VEC_ELEMENTS  *sizeof(float)); 

    v[0] = x;
    v[1] = y;
    v[2] = z;
    return v;
}

void delete_vector(float *v) {
    free(v);
}

/*
 *Function: set_zero
 *----------------------------
 *  Sets given vector components to zero
 */
void set_zero(float *vec)
{
    vec[0] = 0.0;
    vec[1] = 0.0;
    vec[2] = 0.0;
}

/*
 *Function: set_vec_from_float
 *----------------------------
 *  Sets all components of given vector to a given float
 *
 *  v: vector to be set to xyz value
 *  xyz: float value
 *
 */
void set_vec_from_float(float *v, const float xyz)
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
void vec_mult(const float *v1, const float *v2, float *res)
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
void vec_pow_inplace(float *v1, const float p)
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
void vec_pow(const float *v1, const float p, float *res)
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
 *  m: scalar of value float
 *  res: result of scalar multiplication
 *
 */
void vec_mult_scalar(const float *v, const float m, float *res)
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
void vec_add(const float *v1, const float *v2, float *res)
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
void vec_add_scalar(const float *v, const float m, float *res)
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
 *  v1: of type float *
 *  v2: of type float*
 *
 *  returns: new vector with subtraction between v1 and v2 
 */
void vec_sub(const float *v1, const float *v2, float *res)
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
float vec_norm(const float *v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}


float vec_norm_squared(const float *v)
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
void vec_normalize(float *v)
{
    float norm = vec_norm(v);
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
float vec_dot(const float *v1, const float *v2)
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
void vec_cross(const float *u, const float *v, float *res)
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
void vec_reflect(const float *v, const float *normal, float *res)
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
void vec_abs(float *v)
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
 *  v1: of type float *
 *  v2: of type float *
 *
 *  returns: absolute valued vector
 */
void vec_max(const float *v1, const float *v2, float *res)
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
 *  v: of type float *
 *  k: of type float *
 *  theta: in radians
 *
 *  returns: a new vecotr rotated around an axis by a given angle
 */
void vec_rotate(float *v, float *k, float theta, float *res)
{
    //based on Euler rodrigues formula
    float cosTheta = cos(theta);
    float first[NR_VEC_ELEMENTS];
    vec_mult_scalar(v, cosTheta, first);
    float second[NR_VEC_ELEMENTS];
    float tmp[NR_VEC_ELEMENTS]; 
    vec_cross(k, v, tmp);
    vec_mult_scalar(tmp, sin(theta), second);
    float third[NR_VEC_ELEMENTS];
    vec_mult_scalar(k, vec_dot(k, v) * (1 - cosTheta), third);

    vec_add(second, third, res);
    vec_add(first, res, res);
}


/*
 *Function: vec_mode
 *----------------------------
 *  modulo of vector 1 by vector 2 element wise
 */
void vec_mod(float *v1, float *v2, float *res){
    res[0] = mod(v1[0], v2[0]);
    res[1] = mod(v1[1], v2[1]);
    res[2] = mod(v1[2], v2[2]);
}
