#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "greatest.h"

// =============
// TEST HEADERS
// =============
#include "approx_baseline_test.h"
#include "baseline_test.h"
#include "vec3_test.h"
#include "simd_vec3_test.h"

GREATEST_MAIN_DEFS();
int main(int argc, char **argv){
    GREATEST_MAIN_BEGIN();

    printf("================================================================\n");
    printf("==============================VEC===============================\n");
    printf("================================================================");
    RUN_SUITE(vec_3_test);
    printf("================================================================\n");
    printf("============================SIMDVEC=============================\n");
    printf("================================================================");
    RUN_SUITE(simd_vec_3_test);

    printf("\n================================================================\n");
    printf("==============================IMG===============================\n");
    printf("================================================================\n");

    RUN_SUITE(baseline_test);
    printf("================================================================\n");
    printf("===========================IMG_APPROX===========================\n");
    printf("================================================================\n");

    RUN_SUITE(approx_baseline_test);
    printf("================================================================\n");
    printf("==============================END===============================\n");
    printf("================================================================\n");

    GREATEST_MAIN_END();   
    return 0;
}

/* AVAILABLE ASSERTIONS. Further details on the github repo linked above.
ASSERT(COND)
ASSERT_FALSE(COND)
ASSERT_EQ(EXPECTED, ACTUAL)
ASSERT_EQ_FMT(EXPECTED, ACTUAL, FORMAT)
ASSERT_IN_RANGE(EXPECTED, ACTUAL, TOLERANCE)
ASSERT_STR_EQ(EXPECTED, ACTUAL)
ASSERT_STRN_EQ(EXPECTED, ACTUAL, SIZE)
ASSERT_MEM_EQ(EXPECTED, ACTUAL, SIZE)
ASSERT_ENUM_EQ(EXPECTED, ACTUAL, ENUM_STR_FUN)
ASSERT_EQUAL_T(EXPECTED, ACTUAL, TYPE_INFO, UDATA)
ASSERT_OR_LONGJMP(COND)
*/
