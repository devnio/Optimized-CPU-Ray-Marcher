#include "geometry/box.h"
#include "vec3.h"
#include "utility.h"

const int nr_box_params = 3;

/*
Params are:  
- params[0..2]: box extents, x, y, z
-> 3 parameters in total
 */
double sdf_box(const double vec_p[NR_VEC_ELEMENTS], double params[])
{
  double v__q[NR_VEC_ELEMENTS];
  v__q[0] = (vec_p[0] < 0) ? (-vec_p[0] - params[0]) : (vec_p[0] - params[0]);
  v__q[1] = (vec_p[1] < 0) ? (-vec_p[1] - params[1]) : (vec_p[1] - params[1]);
  v__q[2] = (vec_p[2] < 0) ? (-vec_p[2] - params[2]) : (vec_p[2] - params[2]);

  double v__max_tmp[NR_VEC_ELEMENTS]; 
  v__max_tmp[0] = v__q[0] < 0.0 ? 0.0 : v__q[0];
  v__max_tmp[1] = v__q[1] < 0.0 ? 0.0 : v__q[1];
  v__max_tmp[2] = v__q[2] < 0.0 ? 0.0 : v__q[2];

  return vec_norm(v__max_tmp) + min(max(v__q[0], max(v__q[1], v__q[2])), 0.0);
}
