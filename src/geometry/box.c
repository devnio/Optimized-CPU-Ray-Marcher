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
  double vec_q[NR_VEC_ELEMENTS];
  vec_q[0] = (vec_p[0] < 0) ? (vec_p[0] * -1 - params[0]) : (vec_p[0] - params[0]);
  vec_q[1] = (vec_p[1] < 0) ? (vec_p[1] * -1 - params[1]) : (vec_p[1] - params[1]);
  vec_q[2] = (vec_p[2] < 0) ? (vec_p[2] * -1 - params[2]) : (vec_p[2] - params[2]);

  double vec_zero[NR_VEC_ELEMENTS];
  set_zero(vec_zero);

  double vec_max_tmp[NR_VEC_ELEMENTS]; 
  vec_max(vec_q, vec_zero, vec_max_tmp);
  return vec_norm(vec_max_tmp) + min(max(vec_q[0], max(vec_q[1], vec_q[2])), 0.0);
}
