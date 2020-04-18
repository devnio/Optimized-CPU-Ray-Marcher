#include "geometry/octahedron.h"
#include "utility.h"

double sdf_octahedron(Vec3 p, Octahedron octahedron)
{
    p = vec_abs(vec_sub(octahedron.c, p));
    double m = p.x + p.y + p.z - octahedron.s;
    Vec3 q;
    
    if( 3.0*p.x < m ) q = new_vector(p.x, p.y, p.z);
    else if( 3.0*p.y < m ) q = new_vector(p.y, p.z, p.x);
    else if( 3.0*p.z < m ) q = new_vector(p.z, p.x, p.y);
    else return m*0.57735027;
    
    float k = clamp( 0.5*(q.z - q.y + octahedron.s), 0.0, octahedron.s); 
    return vec_norm(new_vector(q.x, q.y-octahedron.s + k, q.z - k)); 
}


Octahedron* build_octahedron(Vec3 c, double s, Material mat){
    Octahedron* oct = (Octahedron*) malloc(sizeof(Octahedron));
    oct->c = c;
    oct->s = s;
    oct->mat = mat;
    return oct;
}