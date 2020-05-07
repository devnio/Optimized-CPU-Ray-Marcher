#ifndef VEC3TEST_H_
#define VEC3TEST_H_

#include "greatest.h"

#define RANDMAX 1e8
#define REPETITIONS 30
#define TOLERANCE 0.000001

SUITE(vec_3_test);

TEST new_vector_test();
TEST vec_mult_test();
TEST vec_mult_scalar_test();
TEST vec_add_test();
TEST vec_add_scalar_test();
TEST vec_sub_test();
TEST vec_norm_test();
TEST vec_normalized_test();
TEST vec_dot_test();
TEST vec_cross_test();
TEST vec_reflect_test();

#endif //VEC3TEST_H_
