#include "geometry/octahedron.h"
#include "utility.h"

const int nr_octahedron_params = 1;

/*
Params are:  
- params[0]: s
-> 1 parameter in total
 */
double sdf_octahedron(double vec_p[NR_VEC_ELEMENTS], double params[])
{

    double v__tmp[NR_VEC_ELEMENTS];
    v__tmp[0] = vec_p[0];
    v__tmp[1] = vec_p[1];
    v__tmp[2] = vec_p[2];

    vec_abs(&v__tmp);
    double m = v__tmp[0] + v__tmp[1] + v__tmp[2] - params[0];
    double v__q[NR_VEC_ELEMENTS];

    if (3.0 * v__tmp[0] < m) {
        v__q[0] = v__tmp[0];
        v__q[1] = v__tmp[1];
        v__q[2] = v__tmp[2];
    } else if (3.0 * v__tmp[1] < m){
        v__q[0] = v__tmp[1];
        v__q[1] = v__tmp[2];
        v__q[2] = v__tmp[0];
    } else if (3.0 * v__tmp[2] < m){
        v__q[0] = v__tmp[2];
        v__q[1] = v__tmp[0];
        v__q[2] = v__tmp[1];    
    } else
        return m * 0.57735027;

    float k = clamp(0.5 * (v__q[2] - v__q[1] + params[0]), 0.0, params[0]);

    double v__val[NR_VEC_ELEMENTS];
    v__val[0] = v__q[0];
    v__val[1] = v__q[1] - params[0] + k;
    v__val[2] = v__q[2] - k;

    return vec_norm(v__val);
}
