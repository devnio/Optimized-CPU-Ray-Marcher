#include "simd.h"
#include <stdio.h>
#include <string.h>
#include <math.h>



#define EXP_POLY_DEGREE 3
#define LOG_POLY_DEGREE 5

#define POLY0(x, c0) SET1_PD(c0)
#define POLY1(x, c0, c1) ADD_PD(MULT_PD(POLY0(x, c1), x), SET1_PD(c0))
#define POLY2(x, c0, c1, c2) ADD_PD(MULT_PD(POLY1(x, c1, c2), x), SET1_PD(c0))
#define POLY3(x, c0, c1, c2, c3) ADD_PD(MULT_PD(POLY2(x, c1, c2, c3), x), SET1_PD(c0))
#define POLY4(x, c0, c1, c2, c3, c4) ADD_PD(MULT_PD(POLY3(x, c1, c2, c3, c4), x), SET1_PD(c0))
#define POLY5(x, c0, c1, c2, c3, c4, c5) ADD_PD(MULT_PD(POLY4(x, c1, c2, c3, c4, c5), x), SET1_PD(c0))

SIMD_MMD exp2d4(SIMD_MMD x)
{
   SIMD_MMI ipart;
   SIMD_MMD dpart, expipart, expdpart;

   x = MIN_PD(x, SET1_PD( 129.00000));
   x = MAX_PD(x, SET1_PD(-126.99999));

   /* ipart = int(x - 0.5) */
   ipart = _mm256_cvtpd_epi32(SUB_PD(x, SET1_PD(0.5)));

   /* dpart = x - ipart */
   dpart = SUB_PD(x, _mm256_cvtepi32_pd(ipart));

   /* expipart = (float) (1 << ipart) */
   expipart = _mm256_cvtps_pd(_mm_castsi128_ps(_mm_slli_epi32(_mm_add_epi32(ipart, _mm_set1_epi32(127)), 23)));

   /* minimax polynomial fit of 2**x, in range [-0.5, 0.5[ */
#if EXP_POLY_DEGREE == 5
   expdpart = POLY5(dpart, 9.9999994e-1, 6.9315308e-1, 2.4015361e-1, 5.5826318e-2, 8.9893397e-3, 1.8775767e-3);
#elif EXP_POLY_DEGREE == 4
   expdpart = POLY4(dpart, 1.0000026, 6.9300383e-1, 2.4144275e-1, 5.2011464e-2, 1.3534167e-2);
#elif EXP_POLY_DEGREE == 3
   expdpart = POLY3(dpart, 9.9992520e-1, 6.9583356e-1, 2.2606716e-1, 7.8024521e-2);
#elif EXP_POLY_DEGREE == 2
   expdpart = POLY2(dpart, 1.0017247, 6.5763628e-1, 3.3718944e-1);
#else
#error
#endif

   return MULT_PD(expipart, expdpart);
}


SIMD_MMD log2d4(SIMD_MMD x)
{
   SIMD_MMI exp = _mm_set1_epi32(0x7F800000);
   SIMD_MMI mant = _mm_set1_epi32(0x007FFFFF);

   __m128 one = _mm_set1_ps(1.0);

   SIMD_MMI i = _mm_castps_si128(_mm256_cvtpd_ps(x));

   SIMD_MMD e = _mm256_cvtepi32_pd(_mm_sub_epi32(_mm_srli_epi32(_mm_and_si128(i, exp), 23), _mm_set1_epi32(127)));

   SIMD_MMD m = _mm256_cvtps_pd(_mm_or_ps(_mm_castsi128_ps(_mm_and_si128(i, mant)), one));

   SIMD_MMD p;

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
   p = MULT_PD(p, SUB_PD(m, SET1_PD(1.0)));

   return ADD_PD(p, e);
}
