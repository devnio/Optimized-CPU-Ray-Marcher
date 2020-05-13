#include <math.h>

#include "vec3.h"
#include "vec3_test.h"

SUITE(vec_3_test){
    RUN_TEST(new_vector_test);
    RUN_TEST(vec_mult_test);
    RUN_TEST(vec_mult_scalar_test);
    RUN_TEST(vec_add_test);
    RUN_TEST(vec_add_scalar_test);
    RUN_TEST(vec_sub_test);
    RUN_TEST(vec_norm_test);
    RUN_TEST(vec_normalized_test);
    RUN_TEST(vec_dot_test);
    RUN_TEST(vec_cross_test);
    RUN_TEST(vec_reflect_test);
}

TEST new_vector_test() {
  double output[NR_VEC_ELEMENTS];
  set_zero(output);
  double input[NR_VEC_ELEMENTS];
  set_zero(input);
  ASSERT_IN_RANGE(input[0], output[0], TOLERANCE);
  ASSERT_IN_RANGE(input[1], output[1], TOLERANCE);
  ASSERT_IN_RANGE(input[2], output[2], TOLERANCE);
  PASS();
}


TEST vec_mult_test() {
  double output[NR_VEC_ELEMENTS];
  set_zero(output);
  double input1[NR_VEC_ELEMENTS];
  double input2[NR_VEC_ELEMENTS];
  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();

      input2[0] = rand();
      input2[1] = rand();
      input2[2] = rand();

      vec_mult(input1, input2, output);
      res1 = input1[0]*input2[0];
      res2 = input1[1]*input2[1];
      res3 = input1[2]*input2[2];

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST vec_mult_scalar_test() {
  double output[NR_VEC_ELEMENTS];
  set_zero(output);
  double input1[NR_VEC_ELEMENTS];
  double input2, res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();
      input2 = rand();

      vec_mult_scalar(input1, input2, output);

      res1 = input1[0]*input2;
      res2 = input1[1]*input2;
      res3 = input1[2]*input2;

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST vec_add_test() {
  double output[NR_VEC_ELEMENTS];
  set_zero(output);
  double input1[NR_VEC_ELEMENTS];
  double input2[NR_VEC_ELEMENTS];
  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();

      input2[0] = rand();
      input2[1] = rand();
      input2[2] = rand();

      vec_add(input1, input2, output);
      res1 = input1[0]+input2[0];
      res2 = input1[1]+input2[1];
      res3 = input1[2]+input2[2];

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST vec_add_scalar_test() {
  double output[NR_VEC_ELEMENTS];
  set_zero(output);
  double input1[NR_VEC_ELEMENTS];
  double input2, res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();
      input2 = rand();

      vec_add_scalar(input1, input2, output);

      res1 = input1[0]+input2;
      res2 = input1[1]+input2;
      res3 = input1[2]+input2;

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST vec_sub_test() {
  double output[NR_VEC_ELEMENTS];
  set_zero(output);
  double input1[NR_VEC_ELEMENTS];
  double input2[NR_VEC_ELEMENTS];
  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();

      input2[0] = rand();
      input2[1] = rand();
      input2[2] = rand();

      vec_sub(input1, input2, output);
      res1 = input1[0]-input2[0];
      res2 = input1[1]-input2[1];
      res3 = input1[2]-input2[2];

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST vec_norm_test() {
  double output = 0.0;
  double input1[NR_VEC_ELEMENTS];
  double res;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();
      
      output = vec_norm(input1);
      res = sqrt(input1[0]*input1[0]+input1[1]*input1[1]+input1[2]*input1[2]);

      ASSERT_IN_RANGE(res, output, TOLERANCE);

  }
  PASS();
}


TEST vec_normalized_test() {
  double output[NR_VEC_ELEMENTS];
  double input1[NR_VEC_ELEMENTS];
  double res1,res2,res3, norm;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();

      vec_normalize(input1);
      output[0] = input1[0];
      output[1] = input1[1];
      output[2] = input1[2];

      norm = sqrt(input1[0]*input1[0]+input1[1]*input1[1]+input1[2]*input1[2]);
      res1=input1[0]/norm;
      res2=input1[1]/norm;
      res3=input1[2]/norm;

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
      
  }
  PASS();
}


TEST vec_dot_test() {
  double output = 0.;
  double input1[NR_VEC_ELEMENTS];
  double input2[NR_VEC_ELEMENTS];
  double res;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();

      input2[0] = rand();
      input2[1] = rand();
      input2[2] = rand();
      
      output = vec_dot(input1, input2);
      res = input1[0]*input2[0]+input1[1]*input2[1]+input1[2]*input2[2];

      ASSERT_IN_RANGE(res, output, TOLERANCE);

  }
  PASS();
}


TEST vec_cross_test() {
  double output[NR_VEC_ELEMENTS];
  set_zero(output);
  double input1[NR_VEC_ELEMENTS];
  double input2[NR_VEC_ELEMENTS];
  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();

      input2[0] = rand();
      input2[1] = rand();
      input2[2] = rand();

      vec_cross(input1, input2, output);
      res1 = input1[1]*input2[2]-input2[1]*input1[2];
      res2 = input1[2]*input2[0]-input2[2]*input1[0];
      res3 = input1[0]*input2[1]-input2[0]*input1[1];

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);

  }
  PASS();
}


//Example of a bad test (?). Dependent on other functions which may be as well erroneous
TEST vec_reflect_test() {
  double output[NR_VEC_ELEMENTS];
  set_zero(output);
  double tmp[NR_VEC_ELEMENTS];
  double tmp1[NR_VEC_ELEMENTS];
  double input1[NR_VEC_ELEMENTS];
  double normal[NR_VEC_ELEMENTS];
  normal[0] = 1.0;
  normal[1] = 0.0;
  normal[2] = 0.0;

  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();     

      vec_reflect(input1, normal, output);
      vec_mult_scalar(normal, vec_dot(normal, input1), tmp1);
      vec_mult_scalar(tmp1, 2.0, tmp1);
      vec_sub(input1, tmp1, tmp);
      res1 = tmp[0];
      res2 = tmp[1];
      res3 = tmp[2];

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);

  }
  PASS();
}  
