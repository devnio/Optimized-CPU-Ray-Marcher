#include <math.h>

#include "vec3.h"
#include "simd_vec3_test.h"

SUITE(simd_vec_3_test){
    RUN_TEST(simd_vec_mult_test);
    RUN_TEST(simd_vec_mult_scalar_test);
    RUN_TEST(simd_vec_add_test);
    RUN_TEST(simd_vec_add_scalar_test);
    RUN_TEST(simd_vec_sub_test);
    RUN_TEST(simd_vec_norm_test);
    RUN_TEST(simd_vec_normalized_test);
    RUN_TEST(simd_vec_dot_test);
    RUN_TEST(simd_vec_cross_test);
    RUN_TEST(simd_vec_reflect_test);
    RUN_TEST(simd_vec_norm_squared_test);
    // RUN_TEST(simd_vec_pow_test);
    RUN_TEST(simd_vec_abs_test);
    RUN_TEST(simd_vec_max_test);
    // RUN_TEST(simd_vec_rotate_test);
    RUN_TEST(simd_vec_mod_test);
}

TEST simd_vec_mult_test() {
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

      SIMD_VEC in1, in2, result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2.x = SET1_PD(input2[0]);
      in2.y = SET1_PD(input2[1]);
      in2.z = SET1_PD(input2[2]);
      simd_vec_mult(&in1, &in2, &result);

      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);


      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST simd_vec_mult_scalar_test() {
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

      SIMD_VEC in1, result;
      SIMD_MMD in2;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2 = SET1_PD(input2);
      simd_vec_mult_scalar(&in1, &in2, &result);
      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST simd_vec_add_test() {
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
      
      
      SIMD_VEC in1, in2, result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2.x = SET1_PD(input2[0]);
      in2.y = SET1_PD(input2[1]);
      in2.z = SET1_PD(input2[2]);
      simd_vec_add(&in1, &in2, &result);
      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST simd_vec_add_scalar_test() {
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

      SIMD_VEC in1, result;
      SIMD_MMD in2;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2 = SET1_PD(input2);

      simd_vec_add_scalar(&in1, &in2, &result);

      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST simd_vec_sub_test() {
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
      SIMD_VEC in1, in2, result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2.x = SET1_PD(input2[0]);
      in2.y = SET1_PD(input2[1]);
      in2.z = SET1_PD(input2[2]);
      simd_vec_sub(&in1, &in2, &result);
      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
  }
  PASS();
}


TEST simd_vec_norm_test() {
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
      SIMD_VEC in1;
      SIMD_MMD result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);

      simd_vec_norm(&in1, &result);
      double res = _mm256_cvtsd_f64(result);


      ASSERT_IN_RANGE(res, output, TOLERANCE);

  }
  PASS();
}


TEST simd_vec_normalized_test() {
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

      SIMD_VEC in1, result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      simd_vec_normalize(&in1, &result);
      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);
      
  }
  PASS();
}


TEST simd_vec_dot_test() {
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
      
      SIMD_VEC in1, in2;
      SIMD_MMD result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2.x = SET1_PD(input2[0]);
      in2.y = SET1_PD(input2[1]);
      in2.z = SET1_PD(input2[2]);
      simd_vec_dot(&in1, &in2, &result);
      double res = _mm256_cvtsd_f64(result);


      ASSERT_IN_RANGE(res, output, TOLERANCE);

  }
  PASS();
}


TEST simd_vec_cross_test() {
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
      SIMD_VEC in1, in2, result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2.x = SET1_PD(input2[0]);
      in2.y = SET1_PD(input2[1]);
      in2.z = SET1_PD(input2[2]);
      simd_vec_cross(&in1, &in2, &result);
      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);

  }
  PASS();
}


//Example of a bad test (?). Dependent on other functions which may be as well erroneous
TEST simd_vec_reflect_test() {
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
      
      SIMD_VEC in1, in2, result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2.x = SET1_PD(normal[0]);
      in2.y = SET1_PD(normal[1]);
      in2.z = SET1_PD(normal[2]);
      simd_vec_reflect(&in1, &in2, &result);
      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);
      
      
      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);

  }
  PASS();
}  


TEST simd_vec_norm_squared_test(){
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

      double output = vec_norm_squared(input1);
      SIMD_VEC in1;
      SIMD_MMD result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      simd_vec_norm_squared(&in1, &result);
      double res = _mm256_cvtsd_f64(result);


      ASSERT_IN_RANGE(res, output, TOLERANCE);


  }
  PASS();
}
// TEST simd_vec_pow_test(){
//     double output[NR_VEC_ELEMENTS];
//   set_zero(output);
//   double input1[NR_VEC_ELEMENTS];
//   double input2;
//   double res1,res2,res3;
//   time_t t;
//   srand((unsigned) time(&t));
//   for(int i=0; i<REPETITIONS; ++i){
//       input1[0] = rand();
//       input1[1] = rand();
//       input1[2] = rand();

//       input2 = rand();
      
//       vec_pow(input1, input2, output);
//       SIMD_VEC in1,  result;
//       SIMD_MMD in2;
//       in1.x = SET1_PD(input1[0]);
//       in1.y = SET1_PD(input1[1]);
//       in1.z = SET1_PD(input1[2]);
//       in2 = SET1_PD(input2);

//       simd_vec_pow(&in1, &in2, &result);
//       double res1 = _mm256_cvtsd_f64(result.x);
//       double res2 = _mm256_cvtsd_f64(result.y);
//       double res3 = _mm256_cvtsd_f64(result.z);

//       ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
//       ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
//       ASSERT_IN_RANGE(res3, output[2], TOLERANCE);

//   }
//   PASS();
// }
TEST simd_vec_abs_test(){
    double output[NR_VEC_ELEMENTS];
  set_zero(output);
  double input1[NR_VEC_ELEMENTS];

  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1[0] = rand();
      input1[1] = rand();
      input1[2] = rand();

      SIMD_VEC in1, in2, result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);

      vec_abs(input1);
      output[0] = input1[0];
      output[1] = input1[1];
      output[2] = input1[2];

      simd_vec_abs(&in1);
      result = in1;
      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);

  }
  PASS();
}
TEST simd_vec_max_test(){
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

      vec_max(input1, input2, output);
      SIMD_VEC in1, in2, result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2.x = SET1_PD(input2[0]);
      in2.y = SET1_PD(input2[1]);
      in2.z = SET1_PD(input2[2]);
      simd_vec_max(&in1, &in2, &result);
      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);

  }
  PASS();
}
// TEST simd_vec_rotate_test(){
//     double output[NR_VEC_ELEMENTS];
//   set_zero(output);
//   double input1[NR_VEC_ELEMENTS];
//   double input2[NR_VEC_ELEMENTS];
//   double res1,res2,res3;
//   time_t t;
//   srand((unsigned) time(&t));
//   for(int i=0; i<REPETITIONS; ++i){
//       input1[0] = rand();
//       input1[1] = rand();
//       input1[2] = rand();

//       input2[0] = rand();
//       input2[1] = rand();
//       input2[2] = rand();

//       vec_rotate(input1, input2,0.123, output);
//       SIMD_VEC in1, in2, result;
//       SIMD_MMD theta = SET1_PD(0.123);
//       in1.x = SET1_PD(input1[0]);
//       in1.y = SET1_PD(input1[1]);
//       in1.z = SET1_PD(input1[2]);
//       in2.x = SET1_PD(input2[0]);
//       in2.y = SET1_PD(input2[1]);
//       in2.z = SET1_PD(input2[2]);
//       simd_vec_rotate(&in1, &in2, &theta, &result);
//       double res1 = _mm256_cvtsd_f64(result.x);
//       double res2 = _mm256_cvtsd_f64(result.y);
//       double res3 = _mm256_cvtsd_f64(result.z);

//       ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
//       ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
//       ASSERT_IN_RANGE(res3, output[2], TOLERANCE);

//   }
//   PASS();
// }
TEST simd_vec_mod_test(){
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

      vec_mod(input1, input2, output);
      SIMD_VEC in1, in2, result;
      in1.x = SET1_PD(input1[0]);
      in1.y = SET1_PD(input1[1]);
      in1.z = SET1_PD(input1[2]);
      in2.x = SET1_PD(input2[0]);
      in2.y = SET1_PD(input2[1]);
      in2.z = SET1_PD(input2[2]);
      simd_vec_mod(&in1, &in2, &result);
      double res1 = _mm256_cvtsd_f64(result.x);
      double res2 = _mm256_cvtsd_f64(result.y);
      double res3 = _mm256_cvtsd_f64(result.z);

      ASSERT_IN_RANGE(res1, output[0], TOLERANCE);
      ASSERT_IN_RANGE(res2, output[1], TOLERANCE);
      ASSERT_IN_RANGE(res3, output[2], TOLERANCE);

  }
  PASS();
}
