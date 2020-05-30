#ifndef SIMD_H_
#define SIMD_H_
#include <immintrin.h>


// Define supported instructions
#define SIMD_V 0

// AVX
#if SIMD_V == 0

#define NR_SIMD_VEC_ELEMS 8


#define SIMD_MMS __m256
#define SIMD_MMD __m256d 
#define SIMD_MMI __m128i

typedef struct
{
    SIMD_MMS x;
    SIMD_MMS y;
    SIMD_MMS z;
} SIMD_VEC;

typedef struct
{
    SIMD_MMS x;
    SIMD_MMS y;
    SIMD_MMS z;
} SIMD_VEC_PS;

extern SIMD_MMS log_base_e;

SIMD_MMS log2f8(SIMD_MMS x);
SIMD_MMS exp2f8(SIMD_MMS x);

// Integers
#define MASKLOAD_EPI _mm_maskload_epi32
#define MASKSTORE_EPI _mm_maskstore_epi32

#define SET_EPI _mm_set_epi32
#define SET1_EPI _mm_set1_epi32

#define MIN_EPI _mm_min_epi32


// Single precision
#define LOAD_PS _mm256_load_ps
#define STORE_PS _mm256_store_ps

#define SET_PS _mm256_set_ps
#define SET1_PS _mm256_set1_ps
#define SET_ZERO_PS _mm256_setzero_ps

#define ADD_PS _mm256_add_ps
#define SUB_PS _mm256_sub_ps
#define MULT_PS _mm256_mul_ps
#define FMADD_PS _mm256_fmadd_ps
#define FMSUB_PS _mm256_fmsub_ps
#define DIV_PS _mm256_div_ps
#define RSQRT_PS _mm256_rsqrt_ps
#define SQRT_PS _mm256_sqrt_ps
#define SQRT_PS_A(x) MULT_PS(x, RSQRT_PS(x))


#define BLENDV_PS _mm256_blendv_ps
#define BLEND_PS _mm256_blend_ps
#define OR_PS _mm256_or_ps
#define XOR_PS _mm256_xor_ps
#define AND_PS _mm256_and_ps
#define ANDNOT_PS _mm256_andnot_ps
#define MAX_PS _mm256_max_ps
#define MIN_PS _mm256_min_ps

#define CONV_PS_TO_EPI _mm256_cvtps_epi32
#define CMP_PS _mm256_cmp_ps
#define MOVEMASK_PS _mm256_movemask_ps


// Double precision
#define LOAD_PD _mm256_load_pd
#define STORE_PD _mm256_store_pd

#define SET_PD _mm256_set_pd
#define SET1_PD _mm256_set1_pd
#define SET_ZERO_PD _mm256_setzero_pd

#define ADD_PD _mm256_add_pd
#define SUB_PD _mm256_sub_pd
#define MULT_PD _mm256_mul_pd
#define DIV_PD _mm256_div_pd
#define SQRT_PD _mm256_sqrt_pd
#define FMADD_PD _mm256_fmadd_pd
#define FMSUB_PD _mm256_fmsub_pd
//#define POW_PD _mm256_pow_pd
#define MAX_PD _mm256_max_pd
//#define COS_PD _mm256_cos_pd
//#define SIN_PD _mm256_sin_pd

//#define LOG_PD _mm256_log_pd

#define BLENDV_PD _mm256_blendv_pd
#define BLEND_PD _mm256_blend_pd
#define OR_PD _mm256_or_pd
#define AND_PD _mm256_and_pd
#define ANDNOT_PD _mm256_andnot_pd
#define MAX_PD _mm256_max_pd
#define MIN_PD _mm256_min_pd

#define CONV_PD_TO_EPI _mm256_cvtpd_epi32
#define CMP_PD _mm256_cmp_pd
#define MOVEMASK_PD _mm256_movemask_pd


#elif SIMD_V == 1
// Add AVX-512 if our machine support it
#endif

#endif // SIMD_H_