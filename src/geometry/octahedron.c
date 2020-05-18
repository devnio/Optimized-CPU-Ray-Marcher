#include "geometry/octahedron.h"
#include "utility.h"

const int nr_octahedron_params = 1;

/*
Params are:  
- params[0]: s
-> 1 parameter in total
 */
double scalar_sdf_octahedron(const double vec_p[NR_VEC_ELEMENTS], double params[])
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



// void simd_sdf_octahedron(const SIMD_VEC* simd_vec, SIMD_MMD* params, SIMD_MMD* res){
//     SIMD_VEC tmp;
//     tmp = *simd_vec; // TODO: are all the struct values also copied?

//     simd_vec_abs(&tmp);
//     SIMD_MMD m = SUB_PD(ADD_PD(tmp.x, ADD_PD(tmp.y,tmp.z)), params[0]);
//     SIMD_MMD mC = MULT_PD(m, SET1_PD(0.33333333333));
    
    
//     //TODO: if else
//     double dm[4], dmC[4];
//     STORE_PD(dm, m);
//     STORE_PD(dmC, mC);
//     double x[4],y[4],z[4];
//     STORE_PD(x, tmp.x);
//     STORE_PD(y, tmp.y);
//     STORE_PD(z, tmp.z);

//     double result[4] = {0.,0.,0.,0.};
//     int flag[4] = {0,0,0,0};

//     int elem =0;
//     if (x[elem] < dmC[elem]){
//         // do nothing
//     }   
//     else if (z[elem] < dmC[elem]){
//         double temp = x[elem];
//         x[elem] = z[elem];
//         z[elem] = y[elem];
//         y[elem] = temp;
//     } 
//     else if(y[elem] < dmC[elem] ){
//         double temp = x[elem];
//         x[elem] = y[elem];
//         y[elem] = z[elem];
//         z[elem] = temp;
//     }
//     else {
//         result[elem] = dm[elem] * 0.57735027;
//         flag[elem] = 1;
//     }
    
//     elem=1;
//     if (x[elem] < dmC[elem]){
//         // do nothing
//     }   
//     else if (z[elem] < dmC[elem]){
//         double temp = x[elem];
//         x[elem] = z[elem];
//         z[elem] = y[elem];
//         y[elem] = temp;
//     } 
//     else if(y[elem] < dmC[elem] ){
//         double temp = x[elem];
//         x[elem] = y[elem];
//         y[elem] = z[elem];
//         z[elem] = temp;
//     }
//     else {
//         result[elem] = dm[elem] * 0.57735027;
//         flag[elem] = 1;
//     }

//     elem=2;
//     if (x[elem] < dmC[elem]){
//         // do nothing
//     }   
//     else if (z[elem] < dmC[elem]){
//         double temp = x[elem];
//         x[elem] = z[elem];
//         z[elem] = y[elem];
//         y[elem] = temp;
//     } 
//     else if(y[elem] < dmC[elem] ){
//         double temp = x[elem];
//         x[elem] = y[elem];
//         y[elem] = z[elem];
//         z[elem] = temp;
//     }
//     else {
//         result[elem] = dm[elem] * 0.57735027;
//         flag[elem] = 1;
//     }

//     elem=3;
//     if (x[elem] < dmC[elem]){
//         // do nothing
//     }   
//     else if (z[elem] < dmC[elem]){
//         double temp = x[elem];
//         x[elem] = z[elem];
//         z[elem] = y[elem];
//         y[elem] = temp;
//     } 
//     else if(y[elem] < dmC[elem] ){
//         double temp = x[elem];
//         x[elem] = y[elem];
//         y[elem] = z[elem];
//         z[elem] = temp;
//     }
//     else {
//         result[elem] = dm[elem] * 0.57735027; 
//         flag[elem] = 1;
//     }
    
    
//     tmp.x = LOAD_PD(x);
//     tmp.y = LOAD_PD(y);
//     tmp.z = LOAD_PD(z);
    

//     SIMD_MMD half = SET1_PD(.5);
//     SIMD_MMD k = MULT_PD(half, SUB_PD( simd_vec->z, ADD_PD(simd_vec->y, params[0])));
//     simd_clamp(&k,SET1_PD(0.),params[0], &k);
//     tmp.y = SUB_PD(tmp.y, ADD_PD(params[0], k));
//     tmp.z = SUB_PD(tmp.z, k);
//     simd_vec_norm(&tmp, res);

//     // TODO clean this hack
//     int loaded = 0;
//     double tmp_res[4];
//     for(int i=0; i<4; ++i){
//         if(flag[i]){
//             if(!loaded){
//                 STORE_PD(tmp_res, *res);
//                 loaded = 1;
//             }
//             tmp_res[i] = result[i];
//         }
//     }
//     *res = LOAD_PD(tmp_res);

// }
void sdf_octahedron(const SIMD_VEC* simd_vec, double params[], SIMD_MMD* res){

    double x[4], y[4], z[4];
    STORE_PD(x, simd_vec->x);
    STORE_PD(y, simd_vec->y);
    STORE_PD(z, simd_vec->z);

    double vec_p0[4] = {x[0], y[0], z[0]};
    double vec_p1[4] = {x[1], y[1], z[1]};
    double vec_p2[4] = {x[2], y[2], z[2]};
    double vec_p3[4] = {x[3], y[3], z[3]};

    //TODO params 0,1,2,3
    

    double res0 =  scalar_sdf_octahedron(vec_p0,  &(params[0]));
    double res1 =  scalar_sdf_octahedron(vec_p1,  &(params[0]));
    double res2 =  scalar_sdf_octahedron(vec_p2,  &(params[0]));
    double res3 =  scalar_sdf_octahedron(vec_p3,  &(params[0]));

    *res = SET_PD(res3, res2, res1, res0);


}

void _sdf_octahedron(const SIMD_VEC* simd_vec, double _params[], SIMD_MMD* res){
    SIMD_VEC tmp;
    tmp = *simd_vec; // TODO: are all the struct values also copied?

    SIMD_MMD params[4];
    params[0] = SET1_PD(_params[0]);

    simd_vec_abs(&tmp);
    SIMD_MMD m = SUB_PD(ADD_PD(tmp.x, ADD_PD(tmp.y,tmp.z)), params[0]);
    SIMD_MMD mC = MULT_PD(m, SET1_PD(0.33333333333));
    
    
    //TODO: if else
    double dm[4], dmC[4];
    STORE_PD(dm, m);
    STORE_PD(dmC, mC);
    double x[4],y[4],z[4];
    STORE_PD(x, tmp.x);
    STORE_PD(y, tmp.y);
    STORE_PD(z, tmp.z);

    double result[4] = {0.,0.,0.,0.};
    int flag[4] = {0,0,0,0};

    int elem =0;
    if (x[elem] < dmC[elem]){
        // do nothing
    }   
    else if (z[elem] < dmC[elem]){
        double temp = x[elem];
        x[elem] = z[elem];
        z[elem] = y[elem];
        y[elem] = temp;
    } 
    else if(y[elem] < dmC[elem] ){
        double temp = x[elem];
        x[elem] = y[elem];
        y[elem] = z[elem];
        z[elem] = temp;
    }
    else {
        result[elem] = dm[elem] * 0.57735027;
        flag[elem] = 1;
    }
    
    elem=1;
    if (x[elem] < dmC[elem]){
        // do nothing
    }   
    else if (z[elem] < dmC[elem]){
        double temp = x[elem];
        x[elem] = z[elem];
        z[elem] = y[elem];
        y[elem] = temp;
    } 
    else if(y[elem] < dmC[elem] ){
        double temp = x[elem];
        x[elem] = y[elem];
        y[elem] = z[elem];
        z[elem] = temp;
    }
    else {
        result[elem] = dm[elem] * 0.57735027;
        flag[elem] = 1;
    }

    elem=2;
    if (x[elem] < dmC[elem]){
        // do nothing
    }   
    else if (z[elem] < dmC[elem]){
        double temp = x[elem];
        x[elem] = z[elem];
        z[elem] = y[elem];
        y[elem] = temp;
    } 
    else if(y[elem] < dmC[elem] ){
        double temp = x[elem];
        x[elem] = y[elem];
        y[elem] = z[elem];
        z[elem] = temp;
    }
    else {
        result[elem] = dm[elem] * 0.57735027;
        flag[elem] = 1;
    }

    elem=3;
    if (x[elem] < dmC[elem]){
        // do nothing
    }   
    else if (z[elem] < dmC[elem]){
        double temp = x[elem];
        x[elem] = z[elem];
        z[elem] = y[elem];
        y[elem] = temp;
    } 
    else if(y[elem] < dmC[elem] ){
        double temp = x[elem];
        x[elem] = y[elem];
        y[elem] = z[elem];
        z[elem] = temp;
    }
    else {
        result[elem] = dm[elem] * 0.57735027; 
        flag[elem] = 1;
    }
    
    
    tmp.x = LOAD_PD(x);
    tmp.y = LOAD_PD(y);
    tmp.z = LOAD_PD(z);
    

    SIMD_MMD half = SET1_PD(.5);
    SIMD_MMD k = MULT_PD(half, SUB_PD( simd_vec->z, ADD_PD(simd_vec->y, params[0])));
    simd_clamp(&k,SET1_PD(0.),params[0], &k);
    tmp.y = SUB_PD(tmp.y, ADD_PD(params[0], k));
    tmp.z = SUB_PD(tmp.z, k);
    simd_vec_norm(&tmp, res);

    // TODO clean this hack
    int loaded = 0;
    double tmp_res[4];
    for(int i=0; i<4; ++i){
        if(flag[i]){
            if(!loaded){
                STORE_PD(tmp_res, *res);
                loaded = 1;
            }
            tmp_res[i] = result[i];
        }
    }
    *res = LOAD_PD(tmp_res);

}
