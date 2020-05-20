#include "geometry/torus.h"
#include "simd.h"

const int nr_torus_params = 2;

/*
Params are:  
- params[0]: r1 squared
- params[1]: r2
-> 2 parameter in total
 */
void sdf_torus(const SIMD_VEC_PS* simd_vec, float _params[], SIMD_MMS* res)
{
    SIMD_MMS params[2];
    params[0] = SET1_PS(_params[0]);
    params[1] = SET1_PS(_params[1]);

    SIMD_MMS x2 = MULT_PS(simd_vec->x,simd_vec->x);
    SIMD_MMS l = FMADD_PS(simd_vec->z,simd_vec->z,x2);
    SIMD_MMS sqrtL = SQRT_PS(l);

    SIMD_MMS pres = FMADD_PS(simd_vec->y,simd_vec->y,l);
    SIMD_MMS m2 = SET1_PS(-2.);
    SIMD_MMS pres2 = FMADD_PS(m2, sqrtL, params[0]);

    SIMD_MMS pres3 = SQRT_PS(ADD_PS(pres,pres2));
    *res = SUB_PS(pres3, params[1]);

}