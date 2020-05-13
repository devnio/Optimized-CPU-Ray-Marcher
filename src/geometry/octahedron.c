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
    vec_abs(&p);
    double m = p.x + p.y + p.z - params[0];

    if (3.0 * p.x < m){

    }    
    else if (3.0 * p.y < m ){
        double temp = p.x;
        p.x = p.y;
        p.y = p.z;
        p.z = temp;
    }
    else if (3.0 * p.z < m){
        double temp = p.x;
        p.x = p.z;
        p.z = p.y;
        p.y = temp;
    }
    else {
        return m * 0.57735027;
    }

    double k = clamp(0.5 * (p.z - p.y + params[0]), 0.0, params[0]);
    p.y = p.y - params[0] + k;
    p.z = p.z - k;
    return vec_norm(&p);
}
