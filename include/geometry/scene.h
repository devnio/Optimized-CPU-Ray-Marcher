#ifndef SCENE_H_
#define SCENE_H_

#include "geometry/plane.h"
#include "geometry/octahedron.h"
#include "geometry/sphere.h"
#include "geometry/box.h"
#include "geometry/cone.h"
#include "geometry/torus.h"

typedef double (*sdf_func)(Vec3 p, Transform transform, double params[]);

typedef struct 
{
    sdf_func sdf;
    double* params;
    Material* mat;
    Transform* transform;

} GeomtericObject;

typedef struct {
    char* name;
    int nr_geom_objs;
    GeomtericObject** geometric_ojects;
} Scene;

typedef struct  
{
    double min_dist;
    int nearest_obj_idx;
    int intersected;
    Vec3 intersection_pt; // if intersected is 0, this shouldn't be used (TODO: maybe separate this?)
    double s;
} SDF_Info;

SDF_Info sdf(Vec3 p, Scene scene, SDF_Info* prev_sdf_info);

Scene* build_scene(int nr_sdfs, const char* name);

#endif // SCENE_H_