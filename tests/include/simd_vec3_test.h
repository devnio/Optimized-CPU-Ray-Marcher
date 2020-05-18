#ifndef SIMD_VEC3TEST_H_
#define SIMD_VEC3TEST_H_

#include "greatest.h"
//#include "simd.h"

#define RANDMAX 1e8
#define REPETITIONS 30
#define TOLERANCE 0.000001

SUITE(simd_vec_3_test);

TEST simd_vec_mult_test();
TEST simd_vec_mult_scalar_test();
TEST simd_vec_add_test();
TEST simd_vec_add_scalar_test();
TEST simd_vec_sub_test();
TEST simd_vec_norm_test();
TEST simd_vec_normalized_test();
TEST simd_vec_dot_test();
TEST simd_vec_cross_test();
TEST simd_vec_reflect_test();

TEST simd_vec_norm_squared_test();
TEST simd_vec_pow_test();
TEST simd_vec_abs_test();
TEST simd_vec_max_test();
TEST simd_vec_rotate_test();
TEST simd_vec_mod_test();

#endif //SIMD_VEC3TEST_H_
