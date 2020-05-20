#include "geometry/octahedron.h"
#include "utility.h"
#include <stdio.h>
#include <stdalign.h>
#include <math.h>

/*
Params are:  
- params[0]: s
-> 1 parameter in total
 */
void sdf_octahedron(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* dists)
{
    const __m256 abs_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x7FFFFFFF));
    SIMD_VEC_PS res;
    res.x = AND_PS(abs_mask, simd_vec_p->x);
    res.y = AND_PS(abs_mask, simd_vec_p->y);
    res.z = AND_PS(abs_mask, simd_vec_p->z);

    SIMD_MMS params0 = SET1_PS(params[0]);

    SIMD_MMS m = SUB_PS(ADD_PS(ADD_PS(res.x, res.y), res.z), params0);
    SIMD_MMS mc = MULT_PS(m, SET1_PS(0.33333333333));

    SIMD_MMS mask_x = CMP_PS(res.x, mc, _CMP_LT_OS);
    SIMD_MMS mask_y = CMP_PS(res.y, mc, _CMP_LT_OS);
    SIMD_MMS mask_z = CMP_PS(res.z, mc, _CMP_LT_OS);

    SIMD_MMS final_mask_0 = ANDNOT_PS(mask_y, SET1_PS(-1));
    SIMD_MMS final_mask_1 = ANDNOT_PS(mask_z, final_mask_0);
    SIMD_MMS final_mask_2 = ANDNOT_PS(mask_x, final_mask_1);
    SIMD_MMS final_mask = ANDNOT_PS(mask_x, final_mask_1);

   
    if (MOVEMASK_PS(final_mask) == 0b11111111)
    {   
        *dists = MULT_PS(m, SET1_PS(0.57735027));
        return;
    }

    SIMD_MMS temp = res.x;
    SIMD_MMS mask_z_2 = ANDNOT_PS(mask_x, mask_z);

    res.x = BLENDV_PS(res.x, res.z, mask_z_2);
    res.z = BLENDV_PS(res.z, res.y, mask_z_2);
    res.y = BLENDV_PS(res.y, temp, mask_z_2);

    mask_y = ANDNOT_PS(mask_x, ANDNOT_PS(mask_z, mask_y));

    res.x = BLENDV_PS(res.x, res.y, mask_y);
    res.y = BLENDV_PS(res.y, res.z, mask_y);
    res.z = BLENDV_PS(res.z, temp, mask_y);

    SIMD_MMS k = MIN_PS(MAX_PS(SET_ZERO_PS(), MULT_PS(SET1_PS(0.5), ADD_PS(SUB_PS(res.z, res.y), params0))), params0);
    res.y = ADD_PS(SUB_PS(res.y, params0), k);
    res.z = SUB_PS(res.z, k);

    SIMD_MMS norm;
    simd_vec_norm(&res, &norm);

    *dists = BLENDV_PS(norm, MULT_PS(m, SET1_PS(0.57735027)), final_mask);
}
