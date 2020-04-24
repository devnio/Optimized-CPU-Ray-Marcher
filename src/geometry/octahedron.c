#include "geometry/octahedron.h"
#include "utility.h"

const int nr_octahedron_params = 1;

/*
Params are:  
- params[0]: s
-> 1 parameter in total
 */
double sdf_octahedron(Vec3 p, double params[])
{
    p = vec_abs(p);
    double m = p.x + p.y + p.z - params[0];
    Vec3 q;
    
    if( 3.0*p.x < m ) q = new_vector(p.x, p.y, p.z);
    else if( 3.0*p.y < m ) q = new_vector(p.y, p.z, p.x);
    else if( 3.0*p.z < m ) q = new_vector(p.z, p.x, p.y);
    else return m*0.57735027;
    
    float k = clamp( 0.5*(q.z - q.y + params[0]), 0.0, params[0]); 
    return vec_norm(new_vector(q.x, q.y-params[0] + k, q.z - k)); 
}
