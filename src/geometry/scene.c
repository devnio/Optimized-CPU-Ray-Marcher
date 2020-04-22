#include <string.h>
#include "geometry/scene.h"

SDF_Info sdf(Vec3 p, Scene scene, SDF_Info* prev_sdf_info)
{
    SDF_Info sdf_info;
    sdf_info.min_dist = INFINITY;

    // Check planes
    for (int k = 0; k < scene.nr_geom_objs; k++)
    {
        // check if has to skip (because it's bouncing off this object)
        if (prev_sdf_info != NULL &&  prev_sdf_info->nearest_obj_idx == k)
            continue;
        
        double dist = scene.geometric_ojects[k]->sdf(p, *(scene.geometric_ojects[k]->transform), scene.geometric_ojects[k]->params);
        if (dist < sdf_info.min_dist)
        {
            sdf_info.min_dist = dist;
            sdf_info.nearest_obj_idx = k;
        }
    }

    return sdf_info;
}

Scene* build_scene(int nr_sdfs, const char* name){

    Scene* scene = (Scene*) malloc(sizeof(Scene));

    scene->name = strdup(name);
    scene->nr_geom_objs = nr_sdfs;
    scene->geometric_ojects = (GeomtericObject**) malloc(sizeof(GeomtericObject*));

    return scene;

}