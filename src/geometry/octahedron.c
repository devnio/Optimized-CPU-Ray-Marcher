#include "geometry/octahedron.h"
#include "utility.h"

const int nr_octahedron_params = 1;

/*
Params are:  
- params[0]: s
-> 1 parameter in total
 */
void sdf_octahedron(const SIMD_VEC* simd_vec_p, double params[], SIMD_MMD* dists)
{
    const __m256d abs_mask = _mm256_castsi256_pd (_mm256_set1_epi64x (0x7FFFFFFFFFFFFFFF));
    SIMD_VEC res;
    res.x = AND_PD (abs_mask, simd_vec_p->x);
    res.y = AND_PD (abs_mask, simd_vec_p->y);
    res.z = AND_PD (abs_mask, simd_vec_p->z);

    SIMD_MMD params0 = SET1_PD(params[0]);

    SIMD_MMD m = SUB_PD(ADD_PD(ADD_PD(res.x, res.y), res.z), params0);
    SIMD_MMD mc = MULT_PD(m, SET1_PD(0.33333333333));

    SIMD_MMD mask_x = CMP_PD(res.x, mc, _CMP_LT_OS);
    SIMD_MMD mask_y = CMP_PD(res.y, mc, _CMP_LT_OS);
    SIMD_MMD mask_z = CMP_PD(res.z, mc, _CMP_LT_OS);

    SIMD_MMD final_mask = ANDNOT_PD(mask_x, ANDNOT_PD(mask_y, ANDNOT_PD(mask_z, SET1_PD(0xFFFFFFFFFFFFFFFF))));

    if (MOVEMASK_PD(final_mask) == 0b1111)
    {
        *dists = MULT_PD(m, SET1_PD(0.57735027));
        return;
    }

    SIMD_MMD temp = ADD_PD(res.x, SET_ZERO_PD());
    mask_z = ANDNOT_PD(mask_x, mask_z);
    res.x = BLENDV_PD(res.x, res.z, mask_z);
    res.z = BLENDV_PD(res.z, res.y, mask_z);
    res.y = BLENDV_PD(res.y, temp, mask_z);

    mask_y = ANDNOT_PD(mask_x, ANDNOT_PD(mask_z, mask_y));
    res.x = BLENDV_PD(res.x, res.y, mask_y);
    res.z = BLENDV_PD(res.y, res.z, mask_y);
    res.y = BLENDV_PD(res.z, temp, mask_y);

    SIMD_MMD k = MIN_PD(MAX_PD(SET_ZERO_PD(), MULT_PD(SET1_PD(0.5), ADD_PD(SUB_PD(res.z, res.y), params0))), params0);
    res.y = ADD_PD(SUB_PD(res.y, params0), k);
    res.z = SUB_PD(res.z, k);

    SIMD_MMD norm;
    simd_vec_norm(&res, &norm);

    *dists = BLENDV_PD(MULT_PD(m, SET1_PD(0.57735027)), norm, final_mask);
}
