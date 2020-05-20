#include "geometry/mandelbulb.h"
#include "utility.h"

const int nr_mandelbulb_params = 0;

SIMD_MMS log_base_e;

/*
Params are:  
- There are no params, mandelbulb spawned at position zero
 */
void sdf_mandelbulb(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists)
{
    SIMD_MMS  x2 = MULT_PS(simd_vec_p->x, simd_vec_p->x);
    SIMD_MMS  y2 = MULT_PS(simd_vec_p->y, simd_vec_p->y);
    SIMD_MMS  z2 = MULT_PS(simd_vec_p->z, simd_vec_p->z);
    SIMD_MMS m = ADD_PS(x2, ADD_PS(y2, z2));

	SIMD_MMS dz = SET1_PS(1.5);

    
    // ==============================//
    // ---- Unrolled Loop Body 1 --- //
    // ==============================//
    SIMD_MMS m2 = MULT_PS(m, m);
    dz = ADD_PS(MULT_PS(SET1_PS(8.0), MULT_PS(m, MULT_PS(m2, MULT_PS(SQRT_PS(m), dz)))), SET1_PS(1.0));

    SIMD_MMS x4 = MULT_PS(x2, x2);
    SIMD_MMS y4 = MULT_PS(y2, y2);
    SIMD_MMS z4 = MULT_PS(z2, z2);

    SIMD_MMS x2z2 = MULT_PS(x2, z2);
    SIMD_MMS x4z4 = ADD_PS(x4, z4);

    SIMD_MMS a = ADD_PS(x2, z2);
    SIMD_MMS a2 = MULT_PS(a, a);
    
    SIMD_MMS b = DIV_PS(SET1_PS(1.0), MULT_PS(a, MULT_PS(a2,SQRT_PS(a)))); 

    SIMD_MMS c = ADD_PS(x4z4, ADD_PS(y4, ADD_PS(MULT_PS(SET1_PS(-6.0), MULT_PS(y2,a)), MULT_PS(SET1_PS(2.0),x2z2)))); 
    
    SIMD_MMS d = ADD_PS(SUB_PS(x2,y2), z2);
    
    SIMD_MMS cb = MULT_PS(c, b);

    SIMD_MMS x = ADD_PS(simd_vec_p->x, MULT_PS(SET1_PS(64.0), MULT_PS(simd_vec_p->x, MULT_PS(simd_vec_p->y, MULT_PS(simd_vec_p->z, MULT_PS(SUB_PS(x2,z2),MULT_PS(d,MULT_PS(ADD_PS(x4, ADD_PS(MULT_PS(SET1_PS(-6.0), x2z2), z4)), cb))))))));
    SIMD_MMS z = ADD_PS(simd_vec_p->z, MULT_PS(SET1_PS(8.0), MULT_PS(simd_vec_p->y,MULT_PS(d, MULT_PS(ADD_PS(MULT_PS(SET1_PS(28.0), MULT_PS(x2z2, x4z4)), SUB_PS(MULT_PS(SUB_PS(MULT_PS(SET1_PS(-70.0),x4),z4), z4), MULT_PS(x4,x4))), cb)))));
    SIMD_MMS y = ADD_PS(simd_vec_p->y, ADD_PS(MULT_PS(MULT_PS(MULT_PS(MULT_PS(SET1_PS(-16.0), y2), a), d),d), MULT_PS(c,c)));

    x2 = MULT_PS(x, x);
    y2 = MULT_PS(y, y);
    z2 = MULT_PS(z, z);
    m = ADD_PS(x2, ADD_PS(y2, z2));


    // -----------
    SIMD_MMS ret_mask;
    SIMD_MMS m_threshold = SET1_PS(256.0);

    ret_mask = CMP_PS(m, m_threshold, _CMP_GT_OS);
    int int_ret_mask = MOVEMASK_PS(ret_mask);

    SIMD_MMS ret_val = MULT_PS(log_base_e, log2d4(m));
    ret_val = DIV_PS(MULT_PS(SET1_PS(0.25), MULT_PS(ret_val, SQRT_PS(m))), dz);
    *simd_mmd_dists = AND_PS(ret_mask, ret_val);

    if(int_ret_mask == 0b11111111) return;
    // -----------

    // ==============================//
    // ---- Unrolled Loop Body 2 --- //
    // ==============================//
    m2 = MULT_PS(m, m);
    dz = ADD_PS(MULT_PS(SET1_PS(8.0), MULT_PS(m, MULT_PS(m2, MULT_PS(SQRT_PS(m), dz)))), SET1_PS(1.0));

    x4 = MULT_PS(x2, x2);
    y4 = MULT_PS(y2, y2);
    z4 = MULT_PS(z2, z2);

    x2z2 = MULT_PS(x2, z2);
    x4z4 = ADD_PS(x4, z4);

    a = ADD_PS(x2, z2);
    a2 = MULT_PS(a, a);
    
    b = DIV_PS(SET1_PS(1.0), MULT_PS(a, MULT_PS(a2,SQRT_PS(a)))); 

    c = ADD_PS(x4z4, ADD_PS(y4, ADD_PS(MULT_PS(SET1_PS(-6.0), MULT_PS(y2,a)), MULT_PS(SET1_PS(2.0),x2z2)))); 
    
    d = ADD_PS(SUB_PS(x2,y2), z2);
    
    cb = MULT_PS(c, b);

    x = ADD_PS(simd_vec_p->x, MULT_PS(SET1_PS(64.0), MULT_PS(x, MULT_PS(y, MULT_PS(z, MULT_PS(SUB_PS(x2,z2),MULT_PS(d,MULT_PS(ADD_PS(x4, ADD_PS(MULT_PS(SET1_PS(-6.0), x2z2), z4)), cb))))))));
    z = ADD_PS(simd_vec_p->z, MULT_PS(SET1_PS(8.0), MULT_PS(y, MULT_PS(d, MULT_PS(ADD_PS(MULT_PS(SET1_PS(28.0), MULT_PS(x2z2, x4z4)), SUB_PS(MULT_PS(SUB_PS(MULT_PS(SET1_PS(-70.0),x4),z4), z4), MULT_PS(x4,x4))), cb)))));
    y = ADD_PS(simd_vec_p->y, ADD_PS(MULT_PS(MULT_PS(MULT_PS(MULT_PS(SET1_PS(-16.0), y2), a), d),d), MULT_PS(c,c)));

    x2 = MULT_PS(x, x);
    y2 = MULT_PS(y, y);
    z2 = MULT_PS(z, z);
    m = ADD_PS(x2, ADD_PS(y2, z2));



    // -----------
    SIMD_MMS ret_mask_2 = CMP_PS(m, m_threshold, _CMP_GT_OS);
    SIMD_MMS ret_mask_tmp = OR_PS(ret_mask, ret_mask_2);
    int_ret_mask = MOVEMASK_PS(ret_mask_tmp);

    ret_mask = ANDNOT_PS(ret_mask, ret_mask_2);

    ret_val = MULT_PS(log_base_e, log2d4(m));
    ret_val = DIV_PS(MULT_PS(SET1_PS(0.25), MULT_PS(ret_val, SQRT_PS(m))), dz);

    *simd_mmd_dists = ADD_PS(*simd_mmd_dists, AND_PS(ret_mask, ret_val));

    if(int_ret_mask == 0b11111111) return;
    // -----------


    // ==============================//
    // ---- Unrolled Loop Body 3 --- //
    // ==============================//
    m2 = MULT_PS(m, m);
    dz = ADD_PS(MULT_PS(SET1_PS(8.0), MULT_PS(m, MULT_PS(m2, MULT_PS(SQRT_PS(m), dz)))), SET1_PS(1.0));

    x4 = MULT_PS(x2, x2);
    y4 = MULT_PS(y2, y2);
    z4 = MULT_PS(z2, z2);

    x2z2 = MULT_PS(x2, z2);
    x4z4 = ADD_PS(x4, z4);

    a = ADD_PS(x2, z2);
    a2 = MULT_PS(a, a);
    
    b = DIV_PS(SET1_PS(1.0), MULT_PS(a, MULT_PS(a2,SQRT_PS(a)))); 
    c = ADD_PS(x4z4, ADD_PS(y4, ADD_PS(MULT_PS(SET1_PS(-6.0), MULT_PS(y2,a)), MULT_PS(SET1_PS(2.0),x2z2)))); 
    
    d = ADD_PS(SUB_PS(x2,y2), z2);
    
    cb = MULT_PS(c, b);

    x = ADD_PS(simd_vec_p->x, MULT_PS(SET1_PS(64.0), MULT_PS(x, MULT_PS(y, MULT_PS(z, MULT_PS(SUB_PS(x2,z2),MULT_PS(d,MULT_PS(ADD_PS(x4, ADD_PS(MULT_PS(SET1_PS(-6.0), x2z2), z4)), cb))))))));
    z = ADD_PS(simd_vec_p->z, MULT_PS(SET1_PS(8.0), MULT_PS(y,MULT_PS(d, MULT_PS(ADD_PS(MULT_PS(SET1_PS(28.0), MULT_PS(x2z2, x4z4)), SUB_PS(MULT_PS(SUB_PS(MULT_PS(SET1_PS(-70.0),x4),z4), z4), MULT_PS(x4,x4))), cb)))));
    y = ADD_PS(simd_vec_p->y, ADD_PS(MULT_PS(MULT_PS(MULT_PS(MULT_PS(SET1_PS(-16.0), y2), a), d),d), MULT_PS(c,c)));

    x2 = MULT_PS(x, x);
    y2 = MULT_PS(y, y);
    z2 = MULT_PS(z, z);
    m = ADD_PS(x2, ADD_PS(y2, z2));



    // -----------
    ret_mask_2 = CMP_PS(m, m_threshold, _CMP_GT_OS);
    SIMD_MMS final_mask = OR_PS(ret_mask_tmp, ret_mask_2);
    int_ret_mask = MOVEMASK_PS(final_mask);
    
    ret_mask = ANDNOT_PS(ret_mask_tmp, ret_mask_2);

    ret_val = SET_ZERO_PS();
    ret_val = MULT_PS(log_base_e, log2d4(m));
    ret_val = DIV_PS(MULT_PS(SET1_PS(0.25), MULT_PS(ret_val, SQRT_PS(m))), dz);
    *simd_mmd_dists = ADD_PS(*simd_mmd_dists, AND_PS(ret_mask, ret_val));

    if(int_ret_mask == 0b11111111) return;
    // -----------


    // ==============================//
    // ---- Unrolled Loop Body 4 --- //
    // ==============================//
    m2 = MULT_PS(m, m);
    dz = ADD_PS(MULT_PS(SET1_PS(8.0), MULT_PS(m, MULT_PS(m2, MULT_PS(SQRT_PS(m), dz)))), SET1_PS(1.0));

    x4 = MULT_PS(x2, x2);
    y4 = MULT_PS(y2, y2);
    z4 = MULT_PS(z2, z2);

    x2z2 = MULT_PS(x2, z2);
    x4z4 = ADD_PS(x4, z4);

    a = ADD_PS(x2, z2);
    a2 = MULT_PS(a, a);
    

    b = DIV_PS(SET1_PS(1.0), MULT_PS(a, MULT_PS(a2,SQRT_PS(a)))); 
    c = ADD_PS(x4z4, ADD_PS(y4, ADD_PS(MULT_PS(SET1_PS(-6.0), MULT_PS(y2,a)), MULT_PS(SET1_PS(2.0),x2z2)))); 
    d = ADD_PS(SUB_PS(x2,y2), z2);
    
    cb = MULT_PS(c, b);

    x = ADD_PS(simd_vec_p->x, MULT_PS(SET1_PS(64.0), MULT_PS(x, MULT_PS(y, MULT_PS(z, MULT_PS(SUB_PS(x2,z2),MULT_PS(d,MULT_PS(ADD_PS(x4, ADD_PS(MULT_PS(SET1_PS(-6.0), x2z2), z4)), cb))))))));
    z = ADD_PS(simd_vec_p->z, MULT_PS(SET1_PS(8.0), MULT_PS(y,MULT_PS(d, MULT_PS(ADD_PS(MULT_PS(SET1_PS(28.0), MULT_PS(x2z2, x4z4)), SUB_PS(MULT_PS(SUB_PS(MULT_PS(SET1_PS(-70.0),x4),z4), z4), MULT_PS(x4,x4))), cb)))));
    y = ADD_PS(simd_vec_p->y, ADD_PS(MULT_PS(MULT_PS(MULT_PS(MULT_PS(SET1_PS(-16.0), y2), a), d),d), MULT_PS(c,c)));

    m = ADD_PS(MULT_PS(x, x), ADD_PS(MULT_PS(y, y), MULT_PS(z, z)));


    // ----------- 
    ret_val = MULT_PS(log_base_e, log2d4(m));
    ret_val = DIV_PS(MULT_PS(SET1_PS(0.25),MULT_PS(ret_val, SQRT_PS(m))), dz);
    *simd_mmd_dists = ADD_PS(*simd_mmd_dists, ANDNOT_PS(final_mask, ret_val));
    // -----------
}