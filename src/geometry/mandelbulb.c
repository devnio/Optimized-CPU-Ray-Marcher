#include "geometry/mandelbulb.h"
#include "utility.h"

const int nr_mandelbulb_params = 0;

/*
Params are:  
- There are no params, mandelbulb spawned at position zero
 */
void sdf_mandelbulb(const SIMD_VEC* simd_vec_p, double params[], SIMD_MMD* simd_mmd_dists)
{

    SIMD_MMD  x2 = MULT_PD(simd_vec_p->x, simd_vec_p->x);
    SIMD_MMD  y2 = MULT_PD(simd_vec_p->y, simd_vec_p->y);
    SIMD_MMD  z2 = MULT_PD(simd_vec_p->z, simd_vec_p->z);
    SIMD_MMD m = ADD_PD(x2, ADD_PD(y2, z2));

	SIMD_MMD dz = SET1_PD(1.5);
    
    // ==============================//
    // ---- Unrolled Loop Body 1 --- //
    // ==============================//
    SIMD_MMD m2 = MULT_PD(m, m);
    dz = MULT_PD(SET1_PD(8.0), MULT_PD(m, MULT_PD(m2, MULT_PD(SQRT_PD(m), dz)))) + SET1_PD(1.0);

    SIMD_MMD x4 = MULT_PD(x2, x2);
    SIMD_MMD y4 = MULT_PD(y2, y2);
    SIMD_MMD z4 = MULT_PD(z2, z2);

    SIMD_MMD x2z2 = MULT_PD(x2, z2);
    SIMD_MMD x4z4 = ADD_PD(x4, z4);

    SIMD_MMD a = ADD_PD(x2, z2);
    SIMD_MMD a2 = MULT_PD(a, a);
    
    // OLD: double b = (a == 0) ? 0.0 : 1.0/sqrt(a*a*a*a*a*a*a); NEW:
    SIMD_MMD b = DIV_PD(SET1_PD(1.0), MULT_PD(a, MULT_PD(a2,SQRT_PD(a)))); // changing from a*a*a*a*a*a*a to a*a2*a4 incurred some floating point errors

    // OLD: double c = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;  NEW:
    SIMD_MMD c = ADD_PD(x4z4, SUB_PD(y4, ADD_PD(MULT_PD(SET1_PD(6.0), MULT_PD(y2,a)), MULT_PD(SET1_PD(2.0),x2z2)))); // incurred some floating point errors
    
    SIMD_MMD d = SUB_PD(x2, ADD_PD(y2, z2));
    
    SIMD_MMD cb = MULT_PD(c, b);

    SIMD_MMD x = ADD_PD(simd_vec_p->x, MULT_PD(SET1_PD(64.0), MULT_PD(simd_vec_p->x, MULT_PD(simd_vec_p->y, MULT_PD(simd_vec_p->z, MULT_PD(SUB_PD(x2,z2),MULT_PD(d,MULT_PD(SUB_PD(x4, ADD_PD(MULT_PD(SET1_PD(6.0), x2z2), z4)), cb))))))));
    // z = vec_p[2] +  -8.0*y*d*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*c*b; // old
    SIMD_MMD z =ADD_PD(simd_vec_p->z, MULT_PD(SET1_PD(8.0), MULT_PD(simd_vec_p->y,MULT_PD(d, MULT_PD(SUB_PD(MULT_PD(MULT_PD(SET1_PD(28.0), x2z2), x4z4), SUB_PD(MULT_PD(ADD_PD(MULT_PD(SET1_PD(70.0),x4),z4), z4), MULT_PD(x4,x4))), cb)))));
    SIMD_MMD y = SUB_PD(simd_vec_p->y, ADD_PD(MULT_PD(MULT_PD(MULT_PD(MULT_PD(SET1_PD(16.0), y2), a), d),d), MULT_PD(c,c)));

    x2 = MULT_PD(x, x);
    y2 = MULT_PD(y, y);
    z2 = MULT_PD(z, z);
    m = ADD_PD(x2, ADD_PD(y2, z2));

    //if(m > 256.0) return 0.25*log(m)*sqrt(m)/dz;


    // ==============================//
    // ---- Unrolled Loop Body 2 --- //
    // ==============================//
    m2 = MULT_PD(m, m);
    dz = MULT_PD(SET1_PD(8.0), MULT_PD(m, MULT_PD(m2, MULT_PD(SQRT_PD(m), dz)))) + SET1_PD(1.0);

    x4 = MULT_PD(x2, x2);
    y4 = MULT_PD(y2, y2);
    z4 = MULT_PD(z2, z2);

    x2z2 = MULT_PD(x2, z2);
    x4z4 = ADD_PD(x4, z4);

    a = ADD_PD(x2, z2);
    a2 = MULT_PD(a, a);
    b = DIV_PD(SET1_PD(1.0), MULT_PD(a, MULT_PD(a2,SQRT_PD(a))));
    c = ADD_PD(x4z4, SUB_PD(y4, ADD_PD(MULT_PD(SET1_PD(6.0), MULT_PD(y2,a)), MULT_PD(SET1_PD(2.0),x2z2)))); 
    d = SUB_PD(x2, ADD_PD(y2, z2)); 
    cb = MULT_PD(c, b);

    x = ADD_PD(simd_vec_p->x, MULT_PD(SET1_PD(64.0), MULT_PD(x, MULT_PD(y, MULT_PD(z, MULT_PD(SUB_PD(x2,z2),MULT_PD(d,MULT_PD(SUB_PD(x4, ADD_PD(MULT_PD(SET1_PD(6.0), x2z2), z4)), cb))))))));
    z = ADD_PD(simd_vec_p->z, MULT_PD(SET1_PD(8.0), MULT_PD(y,MULT_PD(d, MULT_PD(SUB_PD(MULT_PD(MULT_PD(SET1_PD(28.0), x2z2), x4z4), SUB_PD(MULT_PD(ADD_PD(MULT_PD(SET1_PD(70.0),x4),z4), z4), MULT_PD(x4,x4))), cb)))));
    y = SUB_PD(simd_vec_p->y, ADD_PD(MULT_PD(MULT_PD(MULT_PD(MULT_PD(SET1_PD(16.0), y2), a), d),d), MULT_PD(c,c)));

    x2 = MULT_PD(x, x);
    y2 = MULT_PD(y, y);
    z2 = MULT_PD(z, z);
    m = ADD_PD(x2, ADD_PD(y2, z2));

    //if(m > 256.0) return 0.25*log(m)*sqrt(m)/dz;

    // ==============================//
    // ---- Unrolled Loop Body 3 --- //
    // ==============================//
    m2 = MULT_PD(m, m);
    dz = MULT_PD(SET1_PD(8.0), MULT_PD(m, MULT_PD(m2, MULT_PD(SQRT_PD(m), dz)))) + SET1_PD(1.0);

    x4 = MULT_PD(x2, x2);
    y4 = MULT_PD(y2, y2);
    z4 = MULT_PD(z2, z2);

    x2z2 = MULT_PD(x2, z2);
    x4z4 = ADD_PD(x4, z4);

    a = ADD_PD(x2, z2);
    a2 = MULT_PD(a, a);
    b = DIV_PD(SET1_PD(1.0), MULT_PD(a, MULT_PD(a2,SQRT_PD(a))));
    c = ADD_PD(x4z4, SUB_PD(y4, ADD_PD(MULT_PD(SET1_PD(6.0), MULT_PD(y2,a)), MULT_PD(SET1_PD(2.0),x2z2)))); 
    d = SUB_PD(x2, ADD_PD(y2, z2)); 
    cb = MULT_PD(c, b);

    x = ADD_PD(simd_vec_p->x, MULT_PD(SET1_PD(64.0), MULT_PD(x, MULT_PD(y, MULT_PD(z, MULT_PD(SUB_PD(x2,z2),MULT_PD(d,MULT_PD(SUB_PD(x4, ADD_PD(MULT_PD(SET1_PD(6.0), x2z2), z4)), cb))))))));
    z = ADD_PD(simd_vec_p->z, MULT_PD(SET1_PD(8.0), MULT_PD(y,MULT_PD(d, MULT_PD(SUB_PD(MULT_PD(MULT_PD(SET1_PD(28.0), x2z2), x4z4), SUB_PD(MULT_PD(ADD_PD(MULT_PD(SET1_PD(70.0),x4),z4), z4), MULT_PD(x4,x4))), cb)))));
    y = SUB_PD(simd_vec_p->y, ADD_PD(MULT_PD(MULT_PD(MULT_PD(MULT_PD(SET1_PD(16.0), y2), a), d),d), MULT_PD(c,c)));

    x2 = MULT_PD(x, x);
    y2 = MULT_PD(y, y);
    z2 = MULT_PD(z, z);
    m = ADD_PD(x2, ADD_PD(y2, z2));


    //if(m > 256.) return 0.25*log(m)*sqrt(m)/dz; 

    // ==============================//
    // ---- Unrolled Loop Body 4 --- //
    // ==============================//
    m2 = MULT_PD(m, m);
    dz = MULT_PD(SET1_PD(8.0), MULT_PD(m, MULT_PD(m2, MULT_PD(SQRT_PD(m), dz)))) + SET1_PD(1.0);

    x4 = MULT_PD(x2, x2);
    y4 = MULT_PD(y2, y2);
    z4 = MULT_PD(z2, z2);

    x2z2 = MULT_PD(x2, z2);
    x4z4 = ADD_PD(x4, z4);

    a = ADD_PD(x2, z2);
    a2 = MULT_PD(a, a);
    b = DIV_PD(SET1_PD(1.0), MULT_PD(a, MULT_PD(a2,SQRT_PD(a))));
    c = ADD_PD(x4z4, SUB_PD(y4, ADD_PD(MULT_PD(SET1_PD(6.0), MULT_PD(y2,a)), MULT_PD(SET1_PD(2.0),x2z2)))); 
    d = SUB_PD(x2, ADD_PD(y2, z2)); 
    cb = MULT_PD(c, b);

    x = ADD_PD(simd_vec_p->x, MULT_PD(SET1_PD(64.0), MULT_PD(x, MULT_PD(y, MULT_PD(z, MULT_PD(SUB_PD(x2,z2),MULT_PD(d,MULT_PD(SUB_PD(x4, ADD_PD(MULT_PD(SET1_PD(6.0), x2z2), z4)), cb))))))));
    z = ADD_PD(simd_vec_p->z, MULT_PD(SET1_PD(8.0), MULT_PD(y,MULT_PD(d, MULT_PD(SUB_PD(MULT_PD(MULT_PD(SET1_PD(28.0), x2z2), x4z4), SUB_PD(MULT_PD(ADD_PD(MULT_PD(SET1_PD(70.0),x4),z4), z4), MULT_PD(x4,x4))), cb)))));
    y = SUB_PD(simd_vec_p->y, ADD_PD(MULT_PD(MULT_PD(MULT_PD(MULT_PD(SET1_PD(16.0), y2), a), d),d), MULT_PD(c,c)));

    m = ADD_PD(MULT_PD(x, x), ADD_PD(MULT_PD(y, y), MULT_PD(z, z)));

    //*simd_mmd_dists = DIV_PD(MULT_PD(SET1_PD(0.25),MULT_PD(log(m), SQRT_PD(m))), dz);
}