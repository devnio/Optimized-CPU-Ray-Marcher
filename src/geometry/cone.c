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
void sdf_cone(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* dists)
{
    SIMD_MMS params0 = SET1_PS(params[0]);
    SIMD_MMS params1 = SET1_PS(params[1]);
    SIMD_MMS params2 = SET1_PS(params[2]);
    SIMD_MMS params3 = SET1_PS(params[3]);
    SIMD_MMS params4 = SET1_PS(params[4]);
    SIMD_MMS params5 = SET1_PS(params[5]);
    
    SIMD_MMS qx = SQRT_PS(ADD_PS(MULT_PS(simd_vec_p->x, simd_vec_p->x), MULT_PS(simd_vec_p->z, simd_vec_p->z))); 
    SIMD_MMS qy = simd_vec_p->y; 

    SIMD_MMS mask = CMP_PS(qy, SET_ZERO_PS(), _CMP_LT_OS);
    SIMD_MMS param0_or_1 = BLENDV_PS(params1, params0, mask);

    SIMD_MMS cax = SUB_PS(qx, MIN_PS(qx, param0_or_1));

    const __m256 abs_mask = _mm256_castsi256_ps(_mm256_set1_epi32 (0x7FFFFFFF));
    SIMD_MMS qy_abs = AND_PS(abs_mask, qy);
    SIMD_MMS cay = SUB_PS(qy_abs, params2);

    SIMD_MMS vsubx = SUB_PS(params1, qx);
    SIMD_MMS vsuby = SUB_PS(params2, qy);

    SIMD_MMS coeff = MIN_PS(MAX_PS(SET_ZERO_PS(), MULT_PS(ADD_PS(MULT_PS(vsubx, params3), MULT_PS(vsuby, params4)), params5)), SET1_PS(1.0));
    SIMD_MMS tmpx = MULT_PS(coeff, params3);
    SIMD_MMS tmpy = MULT_PS(coeff, params4);

    SIMD_MMS vsubx2 = SUB_PS(qx, params1);
    SIMD_MMS vsuby2 = SUB_PS(qy, params2);

    SIMD_MMS cbx = ADD_PS(vsubx2, tmpx);
    SIMD_MMS cby = ADD_PS(vsuby2, tmpy);

    SIMD_MMS dot_ca = ADD_PS(MULT_PS(cax,cax), MULT_PS(cay,cay));
    SIMD_MMS dot_cb = ADD_PS(MULT_PS(cbx,cbx), MULT_PS(cby,cby));

    SIMD_MMS mask_cbx = CMP_PS(cbx, SET_ZERO_PS(), _CMP_LT_OS);
    SIMD_MMS mask_cay = CMP_PS(cay, SET_ZERO_PS(), _CMP_LT_OS);
    SIMD_MMS mask_cay_and_cbx = AND_PS(mask_cbx, mask_cay);

    SIMD_MMS s = BLENDV_PS(SET1_PS(1.0), SET1_PS(-1.0), mask_cay_and_cbx);
    *dists = MULT_PS(s, SQRT_PS(MIN_PS(dot_ca, dot_cb)));
}
