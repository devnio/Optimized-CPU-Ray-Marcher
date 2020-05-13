#include <string.h>
#include <stdio.h>
#include "geometry/scene.h"
#include "utility.h"
#include "camera.h"

void sdf(Vec3 p, Scene* scene, SDF_Info *sdf_info_out)
{
    // First iteration unrolled so we don't have INFINITY.
    Vec3 t = p;
    apply_transform(&t, scene->geometric_ojects[0]->transform);
    double dist = scene->geometric_ojects[0]->sdf(t, scene->geometric_ojects[0]->params);
    sdf_info_out->min_dist = dist;
    sdf_info_out->nearest_obj_idx = 0;

    // Check geometric objects
    for (int k = 1; k < scene->nr_geom_objs; k++)
    {
        t = p;
        apply_transform(&t, scene->geometric_ojects[k]->transform);
        dist = scene->geometric_ojects[k]->sdf(t, scene->geometric_ojects[k]->params);
        if (dist < sdf_info_out->min_dist)
        {
            sdf_info_out->min_dist = dist;
            sdf_info_out->nearest_obj_idx = k;
        }
    }
}

void apply_transform(Vec3 *p, const Transform *tr)
{
    // apply translation
    vec_sub(p, &tr->center, p);

    if(tr->orientation[1] != 0.0 || tr->orientation[3] != 0.0 || tr->orientation[5] != 0.0){
        // apply rotation
        rotate_point_xyz(p, tr->orientation);
    }

#if INFINITE_REP == 1
    Vec3 c = new_vector(7.5,7.5,7.5);
    t = vec_sub(vec_mod(vec_add(t, vec_mult_scalar(c, 0.5)),c), vec_mult_scalar(c, 0.5));
#endif
}

void create_image(Scene* scene, unsigned int width, unsigned int height) {
    size_t png_img_size = width * height * 4 * sizeof(unsigned char);
    scene->img = (unsigned char *)malloc(png_img_size);
}

void destroy_image(Scene* scene) {
    free(scene->img);
}
