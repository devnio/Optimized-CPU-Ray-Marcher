#include "simd.h"
#include <stdio.h>
#include <string.h>
#include <math.h>



#define EXP_POLY_DEGREE 4
#define LOG_POLY_DEGREE 6

#define POLY0(x, c0) SET1_PS(c0)
#define POLY1(x, c0, c1) ADD_PS(MULT_PS(POLY0(x, c1), x), SET1_PS(c0))
#define POLY2(x, c0, c1, c2) ADD_PS(MULT_PS(POLY1(x, c1, c2), x), SET1_PS(c0))
#define POLY3(x, c0, c1, c2, c3) ADD_PS(MULT_PS(POLY2(x, c1, c2, c3), x), SET1_PS(c0))
#define POLY4(x, c0, c1, c2, c3, c4) ADD_PS(MULT_PS(POLY3(x, c1, c2, c3, c4), x), SET1_PS(c0))
#define POLY5(x, c0, c1, c2, c3, c4, c5) ADD_PS(MULT_PS(POLY4(x, c1, c2, c3, c4, c5), x), SET1_PS(c0))

SIMD_MMS exp2f8(SIMD_MMS x)
{
   __m256i ipart;
   SIMD_MMS fpart, expipart, expfpart;

   x = MIN_PS(x, SET1_PS( 129.00000));
   x = MAX_PS(x, SET1_PS(-126.99999));

   /* ipart = int(x - 0.5) */
   ipart = _mm256_cvtps_epi32(SUB_PS(x, SET1_PS(0.5)));

   /* fpart = x - ipart */
   fpart = SUB_PS(x, _mm256_cvtepi32_ps(ipart));

   /* expipart = (float) (1 << ipart) */
   expipart = _mm256_castsi256_ps(_mm256_slli_epi64(_mm256_add_epi32(ipart, _mm256_set1_epi32(127)), 23));

   /* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
#if EXP_POLY_DEGREE == 5
   expfpart = POLY5(fpart, 9.9999994e-1, 6.9315308e-1, 2.4015361e-1, 5.5826318e-2, 8.9893397e-3, 1.8775767e-3);
#elif EXP_POLY_DEGREE == 4
   expfpart = POLY4(fpart, 1.0000026, 6.9300383e-1, 2.4144275e-1, 5.2011464e-2, 1.3534167e-2);
#elif EXP_POLY_DEGREE == 3
   expfpart = POLY3(fpart, 9.9992520e-1, 6.9583356e-1, 2.2606716e-1, 7.8024521e-2);
#elif EXP_POLY_DEGREE == 2
   expfpart = POLY2(fpart, 1.0017247, 6.5763628e-1, 3.3718944e-1);
#else
#error
#endif

   return MULT_PS(expipart, expfpart);
}


SIMD_MMS log2f8(SIMD_MMS x)
{
   __m256i exp = _mm256_set1_epi32(0x7F800000);
   __m256i mant = _mm256_set1_epi32(0x007FFFFF);

   __m256 one = _mm256_set1_ps(1.0);

   __m256i i = _mm256_castps_si256(x);

   SIMD_MMS e = _mm256_cvtepi32_ps(_mm256_sub_epi32(_mm256_srli_epi32(_mm256_and_si256(i, exp), 23), _mm256_set1_epi32(127)));

   SIMD_MMS m = _mm256_or_ps(_mm256_castsi256_ps(_mm256_and_si256(i, mant)), one);

   SIMD_MMS p;

   /* Minimax polynomial fit of log2(x)/(x - 1), for x in range [1, 2[ */
#if LOG_POLY_DEGREE == 6
   p = POLY5( m, 3.1157899, -3.3241990, 2.5988452, -1.2315303,  3.1821337e-1, -3.4436006e-2);
#elif LOG_POLY_DEGREE == 5
   p = POLY4(m, 2.8882704548164776201, -2.52074962577807006663, 1.48116647521213171641, -0.465725644288844778798, 0.0596515482674574969533);
#elif LOG_POLY_DEGREE == 4
   p = POLY3(m, 2.61761038894603480148, -1.75647175389045657003, 0.688243882994381274313, -0.107254423828329604454);
#elif LOG_POLY_DEGREE == 3
   p = POLY2(m, 2.28330284476918490682, -1.04913055217340124191, 0.204446009836232697516);
#else
#error
#endif

   /* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0*/
   p = MULT_PS(p, SUB_PS(m, SET1_PS(1.0)));

   return ADD_PS(p, e);
}
