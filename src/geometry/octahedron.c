#include "geometry/octahedron.h"
#include "utility.h"

const int nr_octahedron_params = 1;

/*
Params are:  
- params[0]: s
-> 1 parameter in total
 */
double sdf_octahedron(const double vec_p[NR_VEC_ELEMENTS], double params[])
{
    double v__tmp[NR_VEC_ELEMENTS];
    v__tmp[0] = vec_p[0];
    v__tmp[1] = vec_p[1];
    v__tmp[2] = vec_p[2];

    vec_abs(v__tmp);
    double m = v__tmp[0] + v__tmp[1] + v__tmp[2] - params[0];
    double mC = m*0.33333333333;

    if (v__tmp[0] < mC){
        // do nothing
    }   
    else if (v__tmp[2] < mC){
        double temp = v__tmp[0];
        v__tmp[0] = v__tmp[2];
        v__tmp[2] = v__tmp[1];
        v__tmp[1] = temp;
    } 
    else if(v__tmp[1] < mC ){
        double temp = v__tmp[0];
        v__tmp[0] = v__tmp[1];
        v__tmp[1] = v__tmp[2];
        v__tmp[2] = temp;
    }
    else {
        return m * 0.57735027;
    }

    float k = clamp(0.5 * (v__tmp[2] - v__tmp[1] + params[0]), 0.0, params[0]);
    v__tmp[1] = v__tmp[1] - params[0] + k;
    v__tmp[2] = v__tmp[2] - k;

    return vec_norm(v__tmp);
}
