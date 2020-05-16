#ifndef SIMD_H_
#define SIMD_H_
#include <immintrin.h>


// Define supported instructions
#define SIMD_V 0

// AVX
#if SIMD_V == 0

#define NR_SIMD_VEC_ELEMS 4

#define SIMD_MMD __m256d
typedef struct
{
    SIMD_MMD x;
    SIMD_MMD y;
    SIMD_MMD z;
} SIMD_VEC;


// Single precision
// #define LOAD_PS _mm256_load_ps
// #define STORE_PS _mm256_store_ps

// #define SET_PS _mm256_set_ps
// #define SET1_PS _mm256_set1_ps

// #define ADD_PS _mm256_add_ps
// #define SUB_PS _mm256_sub_ps
// #define MULT_PS _mm256_mul_ps
// #define SQRT_PS _mm256_sqrt_ps

// #define AND_PS _mm256_and_ps
// #define ANDNOT_PS _mm256_andnot_ps

// Double precision
#define LOAD_PD _mm256_load_pd
#define STORE_PD _mm256_store_pd

#define SET_PD _mm256_set_pd
#define SET1_PD _mm256_set1_pd

#define ADD_PD _mm256_add_pd
#define SUB_PD _mm256_sub_pd
#define MULT_PD _mm256_mul_pd
#define SQRT_PD _mm256_sqrt_pd

#define AND_PD _mm256_and_pd
#define ANDNOT_PD _mm256_andnot_pd


#elif SIMD_V == 1
// Add AVX-512 if our machine support it
#endif

#endif // SIMD_H_