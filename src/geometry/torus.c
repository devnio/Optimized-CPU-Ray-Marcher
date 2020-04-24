#include "geometry/torus.h"

const int nr_torus_params = 2;

/*
Params are:  
- params[0]: r1
- params[1]: r2
-> 2 parameter in total
 */
double sdf_torus(Vec3 p, double params[]){
    double length = sqrt(p.x * p.x + p.z * p.z);
    Vec3 q = new_vector(length - params[0], p.y, 0.0);
    return sqrt(q.x * q.x + q.y * q.y) - params[1]; 

}
