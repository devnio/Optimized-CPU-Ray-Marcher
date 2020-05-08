#include "geometry/box.h"
#include "vec3.h"
#include "utility.h"

const int nr_box_params = 3;

/*
Params are:  
- params[0..2]: box extents, x, y, z
-> 3 parameters in total
 */
double sdf_box(Vec3 p, double params[])
{
  Vec3 q;
  q.x = (p.x < 0) ? (p.x * -1 - params[0]) : (p.x - params[0]);
  q.y = (p.y < 0) ? (p.y * -1 - params[1]) : (p.y - params[1]);
  q.z = (p.z < 0) ? (p.z * -1 - params[2]) : (p.z - params[2]);

  // Vec3 zero = new_vector_one(0.0);
  Vec3 zero;
  set_vec_from_double(&zero, 0.0);
  Vec3 max_tmp = vec_max(q, zero);
  return vec_norm(&max_tmp) + min(max(q.x, max(q.y, q.z)), 0.0);
}
