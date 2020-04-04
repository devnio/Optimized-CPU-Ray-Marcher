#include "sphere.h"



/*
 * Function: sphere_ray_intersection
 * ----------------------------
 *   Returns the square of the largest of its two input values
 *
 *   o: origin point of type Vec3 
 *   dir: direction vector of type Vec3
 *   s: sphere of type Sphere
 *
 *   returns: the intersection point as a new vector
 */
double sphere_ray_intersection(Vec3 o, Vec3 dir, Sphere s)
{
    Vec3 l = vec_sub(s.c, o);
    float tca = vec_dot(l, dir);
    if (tca < 0)
        return -1;

    float d2 = vec_dot(l, l) - tca * tca;
    if (d2 > s.r2)
        return -1;

    float thc = sqrt(s.r2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 0)
        return t1;
    else
        return t0;
}


/*
 * Function: sdf
 * ----------------------------
 *   Computes the signed distant function 
 *
 *   TODO
 *
 *   returns: TODO
 */
double sdf_sphere(Vec3 p, Sphere s)
{
    return vec_norm(vec_sub(s.c, p)) - s.r;
}
