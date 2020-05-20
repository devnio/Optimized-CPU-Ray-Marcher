#include "geometry/cone.h"
#include "utility.h"

const int nr_cone_params = 3;

/*
Params are:  
- params[0]: r1
- params[1]: r2
- params[2]: h
-> 3 parameters in total
 */
void sdf_cone(const SIMD_VEC* simd_vec_p, double params[], SIMD_MMD* dists)
{
    SIMD_MMD params0 = SET1_PD(params[0]);
    SIMD_MMD params1 = SET1_PD(params[1]);
    SIMD_MMD params2 = SET1_PD(params[2]);
    SIMD_MMD params3 = SET1_PD(params[3]);
    SIMD_MMD params4 = SET1_PD(params[4]);
    SIMD_MMD params5 = SET1_PD(params[5]);
    
    SIMD_MMD qx = SQRT_PD(ADD_PD(MULT_PD(simd_vec_p->x, simd_vec_p->x), MULT_PD(simd_vec_p->z, simd_vec_p->z))); 
    SIMD_MMD qy = simd_vec_p->y; 

    SIMD_MMD mask = CMP_PD(qy, SET_ZERO_PD(), _CMP_LT_OS);
    SIMD_MMD param0_or_1 = BLENDV_PD(params1, params0, mask);

    SIMD_MMD cax = SUB_PD(qx, MIN_PD(qx, param0_or_1));

    const __m256d abs_mask = _mm256_castsi256_pd (_mm256_set1_epi64x (0x7FFFFFFFFFFFFFFF));
    SIMD_MMD qy_abs = AND_PD (abs_mask, qy);
    SIMD_MMD cay = SUB_PD(qy_abs, params2);

    SIMD_MMD vsubx = SUB_PD(params1, qx);
    SIMD_MMD vsuby = SUB_PD(params2, qy);

    SIMD_MMD coeff = MIN_PD(MAX_PD(SET_ZERO_PD(), MULT_PD(ADD_PD(MULT_PD(vsubx, params3), MULT_PD(vsuby, params4)), params5)), SET1_PD(1.0));
    SIMD_MMD tmpx = MULT_PD(coeff, params3);
    SIMD_MMD tmpy = MULT_PD(coeff, params4);

    SIMD_MMD vsubx2 = SUB_PD(qx, params1);
    SIMD_MMD vsuby2 = SUB_PD(qy, params2);

    SIMD_MMD cbx = ADD_PD(vsubx2, tmpx);
    SIMD_MMD cby = ADD_PD(vsuby2, tmpy);

    SIMD_MMD dot_ca = ADD_PD(MULT_PD(cax,cax), MULT_PD(cay,cay));
    SIMD_MMD dot_cb = ADD_PD(MULT_PD(cbx,cbx), MULT_PD(cby,cby));

    SIMD_MMD mask_cbx = CMP_PD(cbx, SET_ZERO_PD(), _CMP_LT_OS);
    SIMD_MMD mask_cay = CMP_PD(cay, SET_ZERO_PD(), _CMP_LT_OS);
    SIMD_MMD mask_cay_and_cbx = AND_PD(mask_cbx, mask_cay);

    SIMD_MMD s = BLENDV_PD(SET1_PD(1.0), SET1_PD(-1.0), mask_cay_and_cbx);
    *dists = MULT_PD(s, SQRT_PD(MIN_PD(dot_ca, dot_cb)));
}
