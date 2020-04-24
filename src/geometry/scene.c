#include <string.h>
#include "geometry/scene.h"

void sdf(Vec3 p, Scene scene, SDF_Info* sdf_info_out)
{
    // First iteration unrolled so we don't have INFINITY.
    double dist = scene.geometric_ojects[0]->sdf(p, *(scene.geometric_ojects[0]->transform), scene.geometric_ojects[0]->params);
    sdf_info_out->min_dist = dist;
    sdf_info_out->nearest_obj_idx = 0;

    // Check geometric objects
    for (int k = 1; k < scene.nr_geom_objs; k++)
    {   
        dist = scene.geometric_ojects[k]->sdf(p, *(scene.geometric_ojects[k]->transform), scene.geometric_ojects[k]->params);
        if (dist < sdf_info_out->min_dist)
        {
            sdf_info_out->min_dist = dist;
            sdf_info_out->nearest_obj_idx = k;
        }
    }
}

Scene* build_scene(const char* name){
    Scene* scene = (Scene*) malloc(sizeof(Scene));

    scene->name = strdup(name);
    scene->nr_geom_objs = 0;
    scene->geometric_ojects = (GeomtericObject**)malloc(sizeof(GeomtericObject*));

    return scene;

}