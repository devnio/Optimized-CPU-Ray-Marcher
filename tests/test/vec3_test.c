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
  Vec3 output = {0.,0.,0.};
  Vec3 input = new_vector(0,0,0);
  ASSERT_IN_RANGE(input.x, output.x, TOLERANCE);
  ASSERT_IN_RANGE(input.y, output.y, TOLERANCE);
  ASSERT_IN_RANGE(input.z, output.z, TOLERANCE);
  PASS();
}


TEST vec_mult_test() {
  Vec3 output = {0.,0.,0.};
  Vec3 input1, input2;
  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      input2 = new_vector(rand(), rand(), rand());

      vec_mult(&input1, &input2, &output);
      res1 = input1.x*input2.x;
      res2 = input1.y*input2.y;
      res3 = input1.z*input2.z;

      ASSERT_IN_RANGE(res1, output.x, TOLERANCE);
      ASSERT_IN_RANGE(res2, output.y, TOLERANCE);
      ASSERT_IN_RANGE(res3, output.z, TOLERANCE);
  }
  PASS();
}


TEST vec_mult_scalar_test() {
  Vec3 output = {0.,0.,0.};
  Vec3 input1;
  double input2, res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      input2 = rand();

      vec_mult_scalar(&input1, input2, &output);

      res1 = input1.x*input2;
      res2 = input1.y*input2;
      res3 = input1.z*input2;

      ASSERT_IN_RANGE(res1, output.x, TOLERANCE);
      ASSERT_IN_RANGE(res2, output.y, TOLERANCE);
      ASSERT_IN_RANGE(res3, output.z, TOLERANCE);
  }
  PASS();
}


TEST vec_add_test() {
  Vec3 output = {0.,0.,0.};
  Vec3 input1, input2;
  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      input2 = new_vector(rand(), rand(), rand());

      output = vec_add(input1, input2);
      res1 = input1.x+input2.x;
      res2 = input1.y+input2.y;
      res3 = input1.z+input2.z;

      ASSERT_IN_RANGE(res1, output.x, TOLERANCE);
      ASSERT_IN_RANGE(res2, output.y, TOLERANCE);
      ASSERT_IN_RANGE(res3, output.z, TOLERANCE);
  }
  PASS();
}


TEST vec_add_scalar_test() {
  Vec3 output = {0.,0.,0.};
  Vec3 input1;
  double input2, res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      input2 = rand();

      vec_add_scalar(&input1, input2, &output);

      res1 = input1.x+input2;
      res2 = input1.y+input2;
      res3 = input1.z+input2;

      ASSERT_IN_RANGE(res1, output.x, TOLERANCE);
      ASSERT_IN_RANGE(res2, output.y, TOLERANCE);
      ASSERT_IN_RANGE(res3, output.z, TOLERANCE);
  }
  PASS();
}


TEST vec_sub_test() {
  Vec3 output = {0.,0.,0.};
  Vec3 input1, input2;
  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      input2 = new_vector(rand(), rand(), rand());

      vec_sub(&input1, &input2, &output);
      res1 = input1.x-input2.x;
      res2 = input1.y-input2.y;
      res3 = input1.z-input2.z;

      ASSERT_IN_RANGE(res1, output.x, TOLERANCE);
      ASSERT_IN_RANGE(res2, output.y, TOLERANCE);
      ASSERT_IN_RANGE(res3, output.z, TOLERANCE);
  }
  PASS();
}


TEST vec_norm_test() {
  double output = 0.;
  Vec3 input1;
  double res;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      
      output = vec_norm(input1);
      res = sqrt(input1.x*input1.x+input1.y*input1.y+input1.z*input1.z);

      ASSERT_IN_RANGE(res, output, TOLERANCE);

  }
  PASS();
}


TEST vec_normalized_test() {
  Vec3 output;
  Vec3 input1;
  double res1,res2,res3, norm;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      
      output = vec_normalized(input1);

      norm = sqrt(input1.x*input1.x+input1.y*input1.y+input1.z*input1.z);
      res1=input1.x/norm;
      res2=input1.y/norm;
      res3=input1.z/norm;

      ASSERT_IN_RANGE(res1, output.x, TOLERANCE);
      ASSERT_IN_RANGE(res2, output.y, TOLERANCE);
      ASSERT_IN_RANGE(res3, output.z, TOLERANCE);
      
  }
  PASS();
}


TEST vec_dot_test() {
  double output = 0.;
  Vec3 input1, input2;
  double res;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      input2 = new_vector(rand(), rand(), rand());
      
      output = vec_dot(input1, input2);
      res = input1.x*input2.x+input1.y*input2.y+input1.z*input2.z;

      ASSERT_IN_RANGE(res, output, TOLERANCE);

  }
  PASS();
}


TEST vec_cross_test() {
  Vec3 output = {0.,0.,0.};
  Vec3 input1, input2;
  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      input2 = new_vector(rand(), rand(), rand());

      output = vec_cross(input1, input2);
      res1 = input1.y*input2.z-input2.y*input1.z;
      res2 = input1.z*input2.x-input2.z*input1.x;
      res3 = input1.x*input2.y-input2.x*input1.y;

      ASSERT_IN_RANGE(res1, output.x, TOLERANCE);
      ASSERT_IN_RANGE(res2, output.y, TOLERANCE);
      ASSERT_IN_RANGE(res3, output.z, TOLERANCE);

  }
  PASS();
}


//Example of a bad test (?). Dependent on other functions which may be as well erroneous
TEST vec_reflect_test() {
  Vec3 output = {0.,0.,0.}, tmp, tmp1;
  Vec3 input1, normal = new_vector(1,0,0);
  double res1,res2,res3;
  time_t t;
  srand((unsigned) time(&t));
  for(int i=0; i<REPETITIONS; ++i){
      input1 = new_vector(rand(), rand(), rand());
      

      output = vec_reflect(input1, normal);
      vec_mult_scalar(&normal, vec_dot(normal, input1), &tmp1);
      vec_mult_scalar(&tmp1, 2.0, &tmp1);
      vec_sub(&input1, &tmp1, &tmp);
      res1 = tmp.x;
      res2 = tmp.y;
      res3 = tmp.z;

      ASSERT_IN_RANGE(res1, output.x, TOLERANCE);
      ASSERT_IN_RANGE(res2, output.y, TOLERANCE);
      ASSERT_IN_RANGE(res3, output.z, TOLERANCE);

  }
  PASS();
}  
