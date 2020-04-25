#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "greatest.h"
/*
*   Based on https://github.com/silentbicycle/greatest
*   Here you can include header files with the tests
*   i.e. #include "test_example.h" 
*/
// INSERT HERE
#include "test_example.h" 
#include "vec3_test.h"


SUITE(main_suite){
    
    /*
    *   Here you can add other test cases
    *   i.e. RUN_TEST(<TEST NAME GOES HERE>);
    */
    // INSERT HERE 
        //Example test
    RUN_TEST(test_sample);

        //Vector tests
        // TODO: maybe add new test case for vectors
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

GREATEST_MAIN_DEFS();
int main(int argc, char **argv){
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(main_suite);

    GREATEST_MAIN_END();  
    return 0;
}
