#include <string.h>
#include <stdio.h>
#include "geometry/scene.h"
#include "camera.h"

void sdf(Vec3 p, Scene scene, SDF_Info *sdf_info_out)
{
    // First iteration unrolled so we don't have INFINITY.
    double dist = scene.geometric_ojects[0]->sdf(apply_transform(p, scene.geometric_ojects[0]->transform), scene.geometric_ojects[0]->params);
    sdf_info_out->min_dist = dist;
    sdf_info_out->nearest_obj_idx = 0;

    // Check geometric objects
    for (int k = 1; k < scene.nr_geom_objs; k++)
    {
        dist = scene.geometric_ojects[k]->sdf(apply_transform(p, scene.geometric_ojects[k]->transform), scene.geometric_ojects[k]->params);
        if (dist < sdf_info_out->min_dist)
        {
            sdf_info_out->min_dist = dist;
            sdf_info_out->nearest_obj_idx = k;
        }
    }
}

Vec3 apply_transform(Vec3 p, const Transform *transform)
{
    // apply translation
    return vec_sub(p, transform->center);

    // TODO: apply rotation
}
