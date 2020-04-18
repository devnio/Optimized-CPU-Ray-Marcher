#include "geometry/cone.h"


double clamp1(double value){
    double min = 0.0;
    double max = 1.0;
    double res = value;
    if (value > max)
        res = max;
    if (value < min)
        res = min;
    return res;
}


double sdf_cone(Vec3 p, Cone c){
    p = vec_sub(p, c.c);
    Vec3 q = new_vector(sqrt(p.x*p.x+p.z*p.z),p.y,0.0);
    Vec3 k1 = new_vector(c.r2, c.h, 0.);
    Vec3 k2 = new_vector(c.r2-c.r1, 2.*c.h, 0);
    Vec3 ca = new_vector(q.x-fmin(q.x, (q.y<0.)?c.r1:c.r2),fabs(q.y)-c.h,0.);
    Vec3 cb = vec_add( vec_sub(q,k1) , vec_mult_scalar(k2,clamp1(vec_dot(vec_sub(k1,q),k2)/vec_dot(k2,k2))));
    double s = (cb.x<0.0 && ca.y<0.0)?-1.:1.0;
    return s*sqrt(fmin(vec_dot(ca,ca),vec_dot(cb,cb)));
}

Cone* build_cone(Vec3 c, double r1, double r2, double h, Material mat){
    Cone* cone = (Cone*) malloc(sizeof(Cone));
    cone->c = c;
    cone->r1 = r1;
    cone->r2 = r2;
    cone->h = h;
    cone->mat = mat;
    return cone;
}
