#include "geometry/torus.h"


double sdf_torus(Vec3 p, Torus torus){
    p = vec_sub(p, torus.c);
    double length = sqrt(p.x * p.x + p.z * p.z);
    Vec3 q = new_vector(length - torus.r1, p.y, 0.0);
    return sqrt(q.x * q.x + q.y * q.y) - torus.r2; 

}

Torus* build_torus(Vec3 c, double r1, double r2, Material mat){
    Torus* torus = (Torus*) malloc(sizeof(Torus));
    torus->c = c;
    torus->r1 = r1;
    torus->r2 = r2;
    torus->mat = mat;
    return torus;
}
