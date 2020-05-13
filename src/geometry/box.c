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
  q.x = (p.x < 0) ? (-p.x - params[0]) : (p.x - params[0]);
  q.y = (p.y < 0) ? (-p.y - params[1]) : (p.y - params[1]);
  q.z = (p.z < 0) ? (-p.z - params[2]) : (p.z - params[2]);

  Vec3 max_tmp; 
  max_tmp.x = q.x < 0.0 ? 0.0 : q.x;
  max_tmp.y = q.y < 0.0 ? 0.0 : q.y;
  max_tmp.z = q.z < 0.0 ? 0.0 : q.z;

  return vec_norm(&max_tmp) + min(max(q.x, max(q.y, q.z)), 0.0);
}
