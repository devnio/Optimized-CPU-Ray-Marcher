#include "geometry/box.h"
#include "vec3.h"
#include "utility.h"

const int nr_box_params = 3;

/*
Params are:  
- params[0..2]: box extents, x, y, z
-> 3 parameters in total
 */
void sdf_box(const SIMD_VEC* simd_vec_p, double params[], SIMD_MMD* simd_mmd_dists)
{
  SIMD_MMD px = SET1_PD(params[0]);
  SIMD_MMD py = SET1_PD(params[1]);
  SIMD_MMD pz = SET1_PD(params[2]);

  //-0.0 represents 1000...000 in binary
  SIMD_MMD signMask = SET1_PD(-0.0);

  //ANDNOT basically causes 0111...111 & x, which forces the sign bit to 0 and gives the absolute value
  SIMD_MMD dx = SUB_PD(ANDNOT_PD(signMask, simd_vec_p->x), px);
  SIMD_MMD dy = SUB_PD(ANDNOT_PD(signMask, simd_vec_p->y), py);
  SIMD_MMD dz = SUB_PD(ANDNOT_PD(signMask, simd_vec_p->z), pz);

  SIMD_MMD zero = SET_ZERO_PD();

  SIMD_VEC v__max_tmp; 
  v__max_tmp.x = MAX_PD(dx, zero);
  v__max_tmp.y = MAX_PD(dy, zero);
  v__max_tmp.z = MAX_PD(dz, zero);

  SIMD_MMD l;
  simd_vec_norm(&v__max_tmp, &l);

  *simd_mmd_dists = ADD_PD(l, MIN_PD(MAX_PD(dx, MAX_PD(dy, dz)), zero));
}
