#include <string.h>
#include <stdio.h>
#include "geometry/scene.h"
#include "utility.h"
#include "camera.h"

void sdf(double vec_p[NR_VEC_ELEMENTS], const Scene *scene, SDF_Info *sdf_info_out)
{
    // First iteration unrolled so we don't have INFINITY.
    double v__transformed_pt[NR_VEC_ELEMENTS];
    apply_transform(vec_p, scene->geometric_ojects[0]->transform, v__transformed_pt);
    double dist = scene->geometric_ojects[0]->sdf(v__transformed_pt, scene->geometric_ojects[0]->params);
    sdf_info_out->min_dist = dist;
    sdf_info_out->nearest_obj_idx = 0;

    // Check geometric objects
    for (int k = 1; k < scene->nr_geom_objs; k++)
    {
        double v__transformed_pt_[NR_VEC_ELEMENTS];
        apply_transform(vec_p, scene->geometric_ojects[k]->transform, v__transformed_pt_);
        dist = scene->geometric_ojects[k]->sdf(v__transformed_pt_, scene->geometric_ojects[k]->params);
        if (dist < sdf_info_out->min_dist)
        {
            sdf_info_out->min_dist = dist;
            sdf_info_out->nearest_obj_idx = k;
        }
    }
}

void apply_transform(double vec_p[NR_VEC_ELEMENTS], const Transform *tr, double vec_res[NR_VEC_ELEMENTS])
{
    // apply translation
    vec_sub(vec_p, tr->center, vec_res);

    if(tr->orientation[1] != 0.0 || tr->orientation[3] != 0.0 || tr->orientation[5] != 0.0){
        // apply rotation
        rotate_point_xyz(vec_res, tr->orientation, vec_res);
    }

/// TODO:
#if INFINITE_REP == 1
    Vec3 c;
    set_vec_from_double(&c, 7.5);
    Vec3 period;
    vec_mult_scalar(&c, 0.5, &period);
    Vec3 add;
    vec_add(&t, &period, &add);
    Vec3 mod;
    vec_mod(&add,&c, &mod);
    vec_sub(&mod, &period, &t);
#endif
}

void create_image(Scene* scene, unsigned int width, unsigned int height) {
    size_t png_img_size = width * height * 4 * sizeof(unsigned char);
    scene->img = (unsigned char *)malloc(png_img_size);
}

void destroy_image(Scene* scene) {
    free(scene->img);
}
