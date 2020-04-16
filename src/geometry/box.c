#include "geometry/box.h"
#include "vec3.h"
#include "utility.h"

float sdf_box(Vec3 p, Box box)
{

  Vec3 d = new_vector(0, -1, 6);
  Vec3 t = vec_sub(p, d);

  Vec3 m = rotate_point(t, new_vector(0, 45, 0));
  m = rotate_point(m, new_vector(45, 0, 0));

  Vec3 q;
  q.x = (m.x < 0) ? (m.x * -1 - box.b.x) : (m.x - box.b.x);
  q.y = (m.y < 0) ? (m.y * -1 - box.b.y) : (m.y - box.b.y);
  q.z = (m.z < 0) ? (m.z * -1 - box.b.z) : (m.z - box.b.z);
  Vec3 zero; zero.x = 0; zero.y = 0; zero.z = 0;



  return vec_norm(vec_max(q, zero));// + min(max(q.x,max(q.y,q.z)),0.0);
}