#include "geometry/box.h"
#include "vec3.h"
#include "utility.h"

const int nr_box_params = 3;

/*
Params are:  
- params[0..2]: box extents, x, y, z
-> 3 parameters in total
 */
void sdf_box(const SIMD_VEC_PS* simd_vec_p, float params[], SIMD_MMS* simd_mmd_dists)
{
  SIMD_MMS px = SET1_PS(params[0]);
  SIMD_MMS py = SET1_PS(params[1]);
  SIMD_MMS pz = SET1_PS(params[2]);

  //-0.0 represents 1000...000 in binary
  SIMD_MMS signMask = SET1_PS(-0.0);

  //ANDNOT basically causes 0111...111 & x, which forces the sign bit to 0 and gives the absolute value
  SIMD_MMS dx = SUB_PS(ANDNOT_PS(signMask, simd_vec_p->x), px);
  SIMD_MMS dy = SUB_PS(ANDNOT_PS(signMask, simd_vec_p->y), py);
  SIMD_MMS dz = SUB_PS(ANDNOT_PS(signMask, simd_vec_p->z), pz);

  SIMD_MMS zero = SET_ZERO_PS();

  SIMD_VEC_PS v__max_tmp; 
  v__max_tmp.x = MAX_PS(dx, zero);
  v__max_tmp.y = MAX_PS(dy, zero);
  v__max_tmp.z = MAX_PS(dz, zero);

  SIMD_MMS l;
  simd_vec_norm(&v__max_tmp, &l);

  *simd_mmd_dists = ADD_PS(l, MIN_PS(MAX_PS(dx, MAX_PS(dy, dz)), zero));
}
