// include the file to test here, i.e. #include "vec3.c"
/*
*   Here we put all our test cases
*   START_TEST(<TEST NAME>) {
*       <TEST FUNCTION GIVEN BY CHECK>(<FUNCTION TO TEST>, <EXPECTED RESUL>);
*       //i.e. ck_assert_str_eq(sayHello(), "Hello");
*   }
*   END_TEST
*
*/


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
TEST test_sample() {
  
  ASSERT_STR_EQ("","");
  PASS(); // or FAIL() or SKIP() depending on what you expect
}

