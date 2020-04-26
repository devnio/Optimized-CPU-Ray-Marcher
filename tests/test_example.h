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

TEST test_sample() {
  
  ASSERT_STR_EQ("","");
  PASS(); // or FAIL() or SKIP() depending on what you expect
}

