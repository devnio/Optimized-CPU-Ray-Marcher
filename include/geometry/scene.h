#ifndef SCENE_H_
#define SCENE_H_

#include "material.h"
#include "geometry/transform.h"

typedef double (*sdf_func)(Vec3 p, double params[]);

typedef struct 
{
    sdf_func sdf;
    double* params;
    const Material* mat;
    const Transform* transform;

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

void sdf(Vec3 p, Scene scene, SDF_Info* sdf_info_out);

Scene* build_scene(const char* name);

Vec3 apply_transform(Vec3 p, const Transform*transform);

#endif // SCENE_H_