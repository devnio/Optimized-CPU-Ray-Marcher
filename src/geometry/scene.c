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
    // Init nearest objects index
    sdf_info_out->nearest_obj_idx = SET1_PD(0.0);

    // Apply transform
    SIMD_VEC simd_vec_transformed_pt;
    apply_transform(simd_vec_p, scene->geometric_ojects[0]->transform, &simd_vec_transformed_pt);

    // Compute distances on current sdf for 4 vectors
    SIMD_MMD simd_mmd_dists;
    scene->geometric_ojects[0]->sdf(&simd_vec_transformed_pt, scene->geometric_ojects[0]->params, &simd_mmd_dists);
    sdf_info_out->min_dist = simd_mmd_dists;

    SIMD_MMD simd_mmd_cmp_mask;
    for (int k = 1; k < scene->nr_geom_objs; k++)
    {
        // Apply transform
        apply_transform(simd_vec_p, scene->geometric_ojects[k]->transform, &simd_vec_transformed_pt);

        // Compute distances on current sdf for 4 vectors
        scene->geometric_ojects[k]->sdf(&simd_vec_transformed_pt, scene->geometric_ojects[k]->params, &simd_mmd_dists);
        simd_mmd_cmp_mask = CMP_PD(sdf_info_out->min_dist, simd_mmd_dists, _CMP_LT_OS);

        // printf("\n\nSDF OUT MIN DIST");
        // debug_simd_mmd(&sdf_info_out->min_dist);

        // printf("\n\nMIN DIST");
        // debug_simd_mmd(&simd_mmd_dists);

        // printf("\n\nMASK");
        // debug_simd_mmd(&simd_mmd_cmp_mask);

        //  -- Update points that have less distance --
        // Zero out mind distances that are bigger according to mask and replace with the smaller ones
        sdf_info_out->min_dist = ADD_PD(AND_PD(simd_mmd_cmp_mask, sdf_info_out->min_dist), 
                                        ANDNOT_PD(simd_mmd_cmp_mask, simd_mmd_dists));

        sdf_info_out->nearest_obj_idx = ADD_PD(AND_PD(simd_mmd_cmp_mask, sdf_info_out->nearest_obj_idx), 
                                        ANDNOT_PD(simd_mmd_cmp_mask, SET1_PD(k)));

        // printf("\n\nUPDATED SDF OUT MIN DIST");
        // debug_simd_mmd(&sdf_info_out->min_dist);
        // printf("\n\n===================================");
    }
}

void apply_transform(SIMD_VEC* simd_vec_p, const Transform *tr, SIMD_VEC* simd_vec_transf_pt)
{
    // apply translation
    // vec_sub(vec_p, tr->center, vec_res);

    simd_vec_sub(simd_vec_p, tr->center, simd_vec_transf_pt);

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
