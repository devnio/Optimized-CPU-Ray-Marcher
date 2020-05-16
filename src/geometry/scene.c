#include <string.h>
#include <stdio.h>
#include <stdalign.h>
#include "geometry/scene.h"
#include "geometry/sphere.h"
#include "utility.h"
#include "camera.h"

// TODO: here I hardcoded the scnee to only contain a sphere 
// when everything works change this to contain all the shapes

void sdf(SIMD_VEC *simd_vec_p, const Scene *scene, SDF_Info *sdf_info_out)
{
    // First iteration unrolled so we don't have INFINITY.
    SIMD_VEC simd_vec_transformed_pt;
    
    apply_transform(simd_vec_p, scene->geometric_ojects[0]->transform, &simd_vec_transformed_pt);

    // debug_simd_vec(&simd_vec_transformed_pt);

    // double dist = scene->geometric_ojects[0]->sdf(v__transformed_pt, scene->geometric_ojects[0]->params);
    SIMD_MMD simd_mmd_dists;
    sdf_sphere(&simd_vec_transformed_pt, scene->geometric_ojects[0]->params, &simd_mmd_dists);
    sdf_info_out->min_dist = simd_mmd_dists;
    // debug_simd_mmd(&sdf_info_out->min_dist);
    sdf_info_out->nearest_obj_idx = SET1_PD(0.0);


    // Check geometric objects TODO: more than one sdf computation
    // for (int k = 1; k < scene->nr_geom_objs; k++)
    // {
    //     double v__transformed_pt_[NR_VEC_ELEMENTS];
    //     apply_transform(vec_p, scene->geometric_ojects[k]->transform, v__transformed_pt_);
    //     dist = scene->geometric_ojects[k]->sdf(v__transformed_pt_, scene->geometric_ojects[k]->params);
    //     if (dist < sdf_info_out->min_dist)
    //     {
    //         sdf_info_out->min_dist = dist;
    //         sdf_info_out->nearest_obj_idx = k;
    //     }
    // }
}

void apply_transform(SIMD_VEC* simd_vec_p, const Transform *tr, SIMD_VEC* simd_vec_transf_pt)
{
    // apply translation
    // vec_sub(vec_p, tr->center, vec_res);

    simd_vec_sub(simd_vec_p, &tr->center, simd_vec_transf_pt);

    // TODO: orientation
    // if(tr->orientation[1] != 0.0 || tr->orientation[3] != 0.0 || tr->orientation[5] != 0.0){
    //     // apply rotation
    //     rotate_point_xyz(vec_res, tr->orientation, vec_res);
    // }

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
