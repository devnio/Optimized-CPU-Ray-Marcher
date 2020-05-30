/*
 * RAYMARHCER Base Class
 * ----------------------------
 *   Authors:
 *    Nihat Isik
 *    Qais EL Okaili
 *    Alexandre Cavaleri
 *    David Graf
 * 
 */
#undef __STRICT_ANSI__ // on win: for M_PI, remove guards in math.h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdalign.h>

#include "config.h"
#include "geometry/scene.h"
#include "camera.h"
#include "utility.h"
#include "light.h"
#include "scene_loader.h"

#include "benchmark/benchmark.h"

#include "simd.h"

// Global Constants
const float const_specularColour[3] = {SPECULAR_COEFF, SPECULAR_COEFF, SPECULAR_COEFF};
const float const_eps_x[3] = {EPSILON_NORMALS, 0, 0};
const float const_eps_y[3] = {0, EPSILON_NORMALS, 0};
const float const_eps_z[3] = {0, 0, EPSILON_NORMALS};

enum Mode
{
    M_RENDER = 0, 
    M_BENCHMARK = 1
}; 

static enum Mode RUN_STATE = M_RENDER;

//===============================
//            CODE
//===============================
void compute_normal(const SIMD_VEC_PS* simd_vec_p, const Scene *scene, SIMD_VEC_PS* simd_vec_normals_out)
{
    // TODO: preallocate this
    SIMD_MMS SIMD_MMD_EPSILON = SET1_PS(EPSILON_NORMALS);

    SIMD_VEC_PS simd_vec_p0;
    simd_vec_p0.x = ADD_PS(simd_vec_p->x, SIMD_MMD_EPSILON);
    simd_vec_p0.y = simd_vec_p->y;
    simd_vec_p0.z = simd_vec_p->z;

    SIMD_VEC_PS simd_vec_p1;
    simd_vec_p1.x = simd_vec_p->x;
    simd_vec_p1.y = ADD_PS(simd_vec_p->y, SIMD_MMD_EPSILON);
    simd_vec_p1.z = simd_vec_p->z;

    SIMD_VEC_PS simd_vec_p2;
    simd_vec_p2.x = simd_vec_p->x; 
    simd_vec_p2.y = simd_vec_p->y;
    simd_vec_p2.z = ADD_PS(simd_vec_p->z, SIMD_MMD_EPSILON);

    // SIMD_VEC for min_dist of normal points
    SIMD_VEC_PS simd_vec_min_dist_p;
    
    // SIMD_VEC min_dist of shifted points
    SIMD_VEC_PS simd_vec_min_dist_p_eps;

    SDF_Info sdf_info;
    sdf_info.finish_ray_mask = SET1_PS(0.0);
    sdf_info.finish_ray_mask_int = 0;

    // Compute sdf for normal points (4 points)
    sdf(simd_vec_p, scene, &sdf_info);
    simd_vec_min_dist_p.x = sdf_info.min_dist;
    simd_vec_min_dist_p.y = sdf_info.min_dist;
    simd_vec_min_dist_p.z = sdf_info.min_dist;

    // Compute sdf for shifted x
    sdf(&simd_vec_p0, scene, &sdf_info);
    simd_vec_min_dist_p_eps.x = sdf_info.min_dist;

    // Compute sdf for shifted y
    sdf(&simd_vec_p1, scene, &sdf_info);
    simd_vec_min_dist_p_eps.y = sdf_info.min_dist;
    
    // Compute sdf for shifted z
    sdf(&simd_vec_p2, scene, &sdf_info);
    simd_vec_min_dist_p_eps.z = sdf_info.min_dist;

    SIMD_VEC_PS sub_res;
    simd_vec_sub(&simd_vec_min_dist_p_eps, &simd_vec_min_dist_p, &sub_res);

    simd_vec_normalize(&sub_res, simd_vec_normals_out);
}

// float compute_specular_coefficient(const float dir[NR_VEC_ELEMENTS], float N[NR_VEC_ELEMENTS], float L[NR_VEC_ELEMENTS], Material *mat)
// {
//     // Light reflected on normal
//     float __R[NR_VEC_ELEMENTS];
//     float __V[NR_VEC_ELEMENTS];
//     vec_mult_scalar(L, -1, __V);
//     vec_reflect(__V, N, __R);
//     vec_mult_scalar(dir, -1, __V);
//     vec_normalize(__V);

//     // Specular term
//     float specAngle = max(vec_dot(__R, __V), 0.0);
//     return pow(specAngle, (*mat).shininess);
// }

void compute_simd_specular_coefficient(const SIMD_VEC_PS* simd_vec_dir, const SIMD_VEC_PS* N, const SIMD_VEC_PS* L, const SIMD_MMS* mat_shininess, SIMD_MMS* spec_coeff_out)
{
    SIMD_VEC_PS R;
    SIMD_VEC_PS V;
    V.x = MULT_PS(SET1_PS(-1.0), L->x);
    V.y = MULT_PS(SET1_PS(-1.0), L->y);
    V.z = MULT_PS(SET1_PS(-1.0), L->z);

    simd_vec_reflect(&V, N, &R);

    V.x = MULT_PS(SET1_PS(-1.0), simd_vec_dir->x);
    V.y = MULT_PS(SET1_PS(-1.0), simd_vec_dir->y);
    V.z = MULT_PS(SET1_PS(-1.0), simd_vec_dir->z);

    SIMD_VEC_PS V_normalized;
    simd_vec_normalize(&V, &V_normalized);

    SIMD_MMS dot;
    simd_vec_dot(&R, &V_normalized, &dot);
    dot = MAX_PS(dot, SET_ZERO_PS());

    simd_mmd_pow_func(&dot, mat_shininess, spec_coeff_out);
}


// void compute_shadow_coefficient(SDF_Info *sdf_info, float *ph, float *t)
// {
//     float mid = sdf_info->min_dist * sdf_info->min_dist;
//     float y = mid / (2.0 * (*ph));
//     float d = sqrt(mid - y * y);
//     sdf_info->s = min(sdf_info->s, LIGHT_STR * d / max(0.0, (*t) - y));
//     *ph = sdf_info->min_dist;
//     *t += sdf_info->min_dist;
// }

void compute_simd_shadow_coefficient(SDF_Info *sdf_info, SIMD_MMS* ph, SIMD_MMS* t, SIMD_MMS* mask)
{
    SIMD_MMS simd_mmd_mid = MULT_PS(sdf_info->min_dist, sdf_info->min_dist);
    SIMD_MMS simd_mmd_y = DIV_PS(simd_mmd_mid, MULT_PS(*ph, SET1_PS(2.0)));
    SIMD_MMS simd_mmd_d = SQRT_PS(SUB_PS(simd_mmd_mid, MULT_PS(simd_mmd_y, simd_mmd_y)));

    SIMD_MMS simd_mmd_tmp0 = MULT_PS(SET1_PS(LIGHT_STR), simd_mmd_d);
    SIMD_MMS simd_mmd_tmp1 = MAX_PS(SET_ZERO_PS(), SUB_PS(*t, simd_mmd_y));

    sdf_info->s = BLENDV_PS(MIN_PS(sdf_info->s, DIV_PS(simd_mmd_tmp0, simd_mmd_tmp1)), sdf_info->s, *mask);

    *ph = sdf_info->min_dist;
    *t = ADD_PS(*t, sdf_info->min_dist);
}

void ray_march(SIMD_VEC_PS *simd_vec_orig, SIMD_VEC_PS *simd_vec_dir, const Scene *scene, const int doShadowSteps, SDF_Info* sdf_info_out)
{
    sdf_info_out->finish_ray_mask_int = 0;
    sdf_info_out->finish_ray_mask = SET1_PS(0.0);
    sdf_info_out->intersected_mask = SET1_PS(0.0);

    SIMD_MMS t = SET1_PS(EPSILON);
    SIMD_MMS ph = SET1_PS(1e20);
    sdf_info_out->s = SET1_PS(1.0);

    SIMD_MMS simd_mmd_THRESHOLD = SET1_PS(INTERSECT_THRESHOLD);
    SIMD_MMS simd_mmd_BBOX_AXES = SET1_PS(BBOX_AXES);

    SIMD_MMS intersection_mask;
    int current_int_intersection_mask = 0;

    SIMD_MMS simd_mmd_norm_sq;
    SIMD_MMS overshoot_mask;
    SIMD_MMS current_overshoot_mask = SET1_PS(0.0);
    int current_int_overshoot_mask = 0;

    SIMD_MMS min_dist_for_marching; // has 0 for points that finished

    // Create marching point
    SIMD_VEC_PS simd_vec_march_pt = *simd_vec_orig;

    for (int i = 0; i < MARCH_COUNT; ++i)
    {
        // Populate sdf_info.min_dist with min distances for the 4 rays
        sdf(&simd_vec_march_pt, scene, sdf_info_out);

        // Compute intersection mask
        // debug_simd_mmd(&sdf_info.min_dist);
        intersection_mask = CMP_PS(sdf_info_out->min_dist, simd_mmd_THRESHOLD, _CMP_LT_OS);
        int int_intersection_mask = MOVEMASK_PS(intersection_mask);
        
        // Don't march with intersected or overshooted rays
        sdf_info_out->finish_ray_mask = OR_PS(sdf_info_out->intersected_mask, current_overshoot_mask);
        min_dist_for_marching = ANDNOT_PS(sdf_info_out->finish_ray_mask, sdf_info_out->min_dist);

        // March
        simd_vec_march_pt.x = ADD_PS(simd_vec_march_pt.x, MULT_PS(simd_vec_dir->x, min_dist_for_marching));
        simd_vec_march_pt.y = ADD_PS(simd_vec_march_pt.y, MULT_PS(simd_vec_dir->y, min_dist_for_marching));
        simd_vec_march_pt.z = ADD_PS(simd_vec_march_pt.z, MULT_PS(simd_vec_dir->z, min_dist_for_marching));

        // EXIT CONDITIONS [TODO: can move before march to precompute final_mask | but we do it like this now like we always did]
        // Check intersection
        if (current_int_intersection_mask != int_intersection_mask)
        {
            current_int_intersection_mask = int_intersection_mask;
            sdf_info_out->intersected_mask = intersection_mask;

            // Compute last shadow coeff only for intersected ray
            SIMD_MMS filter_intersected_rays = XOR_PS(intersection_mask, sdf_info_out->intersected_mask);
            // Shadow
            if (doShadowSteps == 1)
            {
                compute_simd_shadow_coefficient(sdf_info_out, &ph, &t, &filter_intersected_rays);
            }

            // printf("\n\n current_int_intersection_mask is: %d", current_int_intersection_mask);
            if (current_int_intersection_mask == 0b11111111)
            {
                // Save intersected ray (min_dist and nearest_obj_idx should already be right)
                sdf_info_out->intersection_pt = simd_vec_march_pt;
                break;
            }
        }

        // Check overshoot
        simd_vec_norm_squared(&simd_vec_march_pt, &simd_mmd_norm_sq);
        overshoot_mask = CMP_PS(simd_mmd_norm_sq, simd_mmd_BBOX_AXES, _CMP_GT_OS);
        int int_overshoot_mask = MOVEMASK_PS(overshoot_mask);
        if (current_int_overshoot_mask != int_overshoot_mask)
        {
            current_overshoot_mask = overshoot_mask;
            current_int_overshoot_mask = int_overshoot_mask;

            if (current_int_overshoot_mask == 0b11111111)
            {
                break;
            }
        }

        // Check combination
        // If all ray in state intersected or overshooted -> exit
        sdf_info_out->finish_ray_mask_int = MOVEMASK_PS(sdf_info_out->finish_ray_mask);
        if (sdf_info_out->finish_ray_mask_int == 0b11111111)
        {
            sdf_info_out->intersection_pt = simd_vec_march_pt;
            break;
        }

        // Shadow
        if (doShadowSteps == 1)
        {
            // Update the finish ray mask for this iteration
            sdf_info_out->finish_ray_mask = OR_PS(sdf_info_out->intersected_mask, overshoot_mask);
            compute_simd_shadow_coefficient(sdf_info_out, &ph, &t, &sdf_info_out->finish_ray_mask);
        }
    }

    sdf_info_out->finish_ray_mask = SET1_PS(1.0);
    sdf_info_out->intersection_pt = simd_vec_march_pt;

}

/*
 * Function: trace
 * ----------------------------
 *   o: origin
 *   dir: direction of ray
 *   sps: scene, spheres
 *   pLight: pointLight that lits the scene
 *   depth: the maximum amount of bounces (for reflective surfaces)
 *   exlcudeSp: the sphere we just bounced off (exclude it in the distance search)
 *
 *   returns: the color of the pixel that generated the direction
 */
void trace(SIMD_VEC_PS *simd_vec_orig,
           SIMD_VEC_PS *simd_vec_dir,
           const Scene *scene,
           const int depth, 
           SIMD_VEC_PS* simd_vec_finalColor)
{
    // CHECK INTERSECTION WITH SCENE
    SDF_Info sdf_info;
    ray_march(simd_vec_orig, simd_vec_dir, scene, 0, &sdf_info);

    // Check if at least one intersection
    if (MOVEMASK_PS(sdf_info.intersected_mask) == 0b00000000)
    {
        return;
    }

    // Get the materials
    alignas(32) float nearest_obj_idx[NR_SIMD_VEC_ELEMS]; 
    STORE_PS(nearest_obj_idx, sdf_info.nearest_obj_idx); // TODO: transform nearest_obj_idx into integer simd vec
    Material mat0 = *(scene->geometric_ojects[(int)nearest_obj_idx[0]]->mat);
    Material mat1 = *(scene->geometric_ojects[(int)nearest_obj_idx[1]]->mat);
    Material mat2 = *(scene->geometric_ojects[(int)nearest_obj_idx[2]]->mat);
    Material mat3 = *(scene->geometric_ojects[(int)nearest_obj_idx[3]]->mat);
    Material mat4 = *(scene->geometric_ojects[(int)nearest_obj_idx[4]]->mat);
    Material mat5 = *(scene->geometric_ojects[(int)nearest_obj_idx[5]]->mat);
    Material mat6 = *(scene->geometric_ojects[(int)nearest_obj_idx[6]]->mat);
    Material mat7 = *(scene->geometric_ojects[(int)nearest_obj_idx[7]]->mat);

    // Create simd vectors for the material properties
    SIMD_MMS simd_mmd_mat_shininess = SET_PS(mat7.shininess, mat6.shininess, mat5.shininess, mat4.shininess, 
                                             mat3.shininess, mat2.shininess, mat1.shininess, mat0.shininess);
    SIMD_MMS simd_mmd_mat_refl = SET_PS(mat7.refl, mat6.refl, mat5.refl, mat4.refl, 
                                        mat3.refl, mat2.refl, mat1.refl, mat0.refl);
    SIMD_VEC_PS simd_vec_mat_surfCol;
    simd_vec_mat_surfCol.x = SET_PS(mat7.surfCol[0], mat6.surfCol[0], mat5.surfCol[0], mat4.surfCol[0], 
                                    mat3.surfCol[0], mat2.surfCol[0], mat1.surfCol[0], mat0.surfCol[0]);
    simd_vec_mat_surfCol.y = SET_PS(mat7.surfCol[1], mat6.surfCol[1], mat5.surfCol[1], mat4.surfCol[1],
                                    mat3.surfCol[1], mat2.surfCol[1], mat1.surfCol[1], mat0.surfCol[1]);
    simd_vec_mat_surfCol.z = SET_PS(mat7.surfCol[2], mat6.surfCol[2], mat5.surfCol[2], mat4.surfCol[2],
                                    mat3.surfCol[2], mat2.surfCol[2], mat1.surfCol[2], mat0.surfCol[2]);

    // Normals
    SIMD_VEC_PS simd_vec_N;
    compute_normal(&sdf_info.intersection_pt, scene, &simd_vec_N);

    // Flip normals simd vectors
    SIMD_MMS simd_mmd_normals_dot;
    simd_vec_dot(simd_vec_dir, &simd_vec_N, &simd_mmd_normals_dot);
    SIMD_MMS simd_mmd_flip_normals_mask = CMP_PS(simd_mmd_normals_dot, SET1_PS(0.0), _CMP_GT_OS);
    
    // Flip normals check for all simd
    // if (MOVEMASK_PS(simd_mmd_flip_normals_mask) != 0b00000000) 
    // {
    //     // Masked mult scalar
    //     SIMD_MMS simd_vec_minus_one = SET1_PS(-1.0);
    //     simd_vec_N.x = BLENDV_PS(simd_vec_N.x, MULT_PS(simd_vec_N.x, simd_vec_minus_one), simd_mmd_flip_normals_mask);
    //     simd_vec_N.y = BLENDV_PS(simd_vec_N.y, MULT_PS(simd_vec_N.y, simd_vec_minus_one), simd_mmd_flip_normals_mask);
    //     simd_vec_N.z = BLENDV_PS(simd_vec_N.z, MULT_PS(simd_vec_N.z, simd_vec_minus_one), simd_mmd_flip_normals_mask);
    // }

    if (depth < MAX_RAY_DEPTH) 
    {
        SIMD_MMS reflectivity_mask = CMP_PS(simd_mmd_mat_refl, SET_ZERO_PS(), _CMP_GT_OS);
        int int_reflectivity_mask = MOVEMASK_PS(reflectivity_mask);

        if(int_reflectivity_mask != 0) {
            SIMD_VEC_PS simd_vec_reflDir;
            SIMD_VEC_PS simd_vec_reflectedCol;
            SIMD_VEC_PS simd_vec_tmp;
            SIMD_MMS simd_mmd_eps = SET1_PS(EPSILON);
            SIMD_MMS simd_mmd_refl_coeff = SET1_PS(REFLECTIVE_COEFF);

            simd_vec_reflectedCol.x = SET_ZERO_PS();
            simd_vec_reflectedCol.y = SET_ZERO_PS();
            simd_vec_reflectedCol.z = SET_ZERO_PS();

            // Compute reflected dir
            simd_vec_reflect(simd_vec_dir, &simd_vec_N, &simd_vec_reflDir);
            simd_vec_normalize(&simd_vec_reflDir, &simd_vec_reflDir);

            // Compute reflected color
            simd_vec_mult_scalar(&simd_vec_N, &simd_mmd_eps, &simd_vec_tmp);
            simd_vec_add(&sdf_info.intersection_pt, &simd_vec_tmp, &simd_vec_tmp);

            trace(&simd_vec_tmp, &simd_vec_reflDir, scene, depth + 1, &simd_vec_reflectedCol);

            simd_vec_mult_scalar(&simd_vec_reflectedCol, &simd_mmd_mat_refl, &simd_vec_tmp);
            simd_vec_mult_scalar(&simd_vec_tmp, &simd_mmd_refl_coeff, simd_vec_finalColor);
        }
    }

    SIMD_VEC_PS simd_vec_L;
    simd_vec_sub(scene->light->c, &sdf_info.intersection_pt, &simd_vec_L);

    // Distance between intersection_pt and light source
    SIMD_MMS simd_mmd_light_dist;
    simd_vec_norm(&simd_vec_L, &simd_mmd_light_dist);

    SIMD_MMS simd_mmd_light_inv_dist = DIV_PS(SET1_PS(1.0), simd_mmd_light_dist); // TODO: use rcp operation

    // Normalize light direction
    simd_vec_L.x = MULT_PS(simd_vec_L.x, simd_mmd_light_inv_dist);
    simd_vec_L.y = MULT_PS(simd_vec_L.y, simd_mmd_light_inv_dist);
    simd_vec_L.z = MULT_PS(simd_vec_L.z, simd_mmd_light_inv_dist);

    // TODO: SHADOWS
    SDF_Info sdf_shadow_info;
    if (scene->nr_geom_objs > 1)
    {
        SIMD_VEC_PS simd_mmd_offset_towards_L;
        simd_mmd_offset_towards_L.x = MULT_PS(SET1_PS(EPSILON), simd_vec_L.x);
        simd_mmd_offset_towards_L.y = MULT_PS(SET1_PS(EPSILON), simd_vec_L.y);
        simd_mmd_offset_towards_L.z = MULT_PS(SET1_PS(EPSILON), simd_vec_L.z);

        simd_mmd_offset_towards_L.x = ADD_PS(sdf_info.intersection_pt.x, simd_mmd_offset_towards_L.x);
        simd_mmd_offset_towards_L.y = ADD_PS(sdf_info.intersection_pt.y, simd_mmd_offset_towards_L.y);
        simd_mmd_offset_towards_L.z = ADD_PS(sdf_info.intersection_pt.z, simd_mmd_offset_towards_L.z);
        
        ray_march(&simd_mmd_offset_towards_L, &simd_vec_L, scene, 1, &sdf_shadow_info);
        sdf_shadow_info.s = MIN_PS(MAX_PS(sdf_shadow_info.s, SET1_PS(SHADOW_LIGHTNESS)), SET1_PS(1.0));
    }

    // Lamber's cosine law
    SIMD_MMS simd_mmd_lambertian;
    simd_vec_dot(&simd_vec_L, &simd_vec_N, &simd_mmd_lambertian);
    simd_mmd_lambertian = MAX_PS(simd_mmd_lambertian, SET_ZERO_PS());
    SIMD_MMS simd_mmd_lambertian_mask = CMP_PS(simd_mmd_lambertian, SET_ZERO_PS(), _CMP_GT_OS);

    // float specular = 0;
    SIMD_MMS simd_mmd_specular = SET_ZERO_PS();
    if (MOVEMASK_PS(simd_mmd_lambertian_mask) != 0b00000000)
    {
        compute_simd_specular_coefficient(simd_vec_dir, &simd_vec_N, &simd_vec_L, &simd_mmd_mat_shininess, &simd_mmd_specular);
        simd_mmd_specular = AND_PS(simd_mmd_specular, simd_mmd_lambertian_mask);
    }

    // START 0 - Compute diffuse color computation
    SIMD_VEC_PS simd_vec_diffuse_color;// = simd_vec_mat_surfCol
    simd_vec_diffuse_color.x = MULT_PS(simd_vec_mat_surfCol.x, simd_mmd_lambertian);
    simd_vec_diffuse_color.y = MULT_PS(simd_vec_mat_surfCol.y, simd_mmd_lambertian);
    simd_vec_diffuse_color.z = MULT_PS(simd_vec_mat_surfCol.z, simd_mmd_lambertian);

    simd_vec_diffuse_color.x = MULT_PS(simd_vec_diffuse_color.x, sdf_shadow_info.s); // TODO: replace 1.0 by s
    simd_vec_diffuse_color.y = MULT_PS(simd_vec_diffuse_color.y, sdf_shadow_info.s); // for soft shadows
    simd_vec_diffuse_color.z = MULT_PS(simd_vec_diffuse_color.z, sdf_shadow_info.s);
    
    simd_vec_diffuse_color.x = MULT_PS(simd_vec_diffuse_color.x, scene->light->emissionColor->x); 
    simd_vec_diffuse_color.y = MULT_PS(simd_vec_diffuse_color.y, scene->light->emissionColor->y); 
    simd_vec_diffuse_color.z = MULT_PS(simd_vec_diffuse_color.z, scene->light->emissionColor->z); 
    
    simd_vec_diffuse_color.x = MULT_PS(simd_vec_diffuse_color.x, simd_mmd_light_inv_dist); 
    simd_vec_diffuse_color.y = MULT_PS(simd_vec_diffuse_color.y, simd_mmd_light_inv_dist); 
    simd_vec_diffuse_color.z = MULT_PS(simd_vec_diffuse_color.z, simd_mmd_light_inv_dist); 
    // END 0 
    
    // START 1 - Compute Ambient Color - TODO: for now assume ambient color is always 0
    SIMD_VEC_PS simd_vec_ambient_color;
    simd_vec_ambient_color.x = SET_ZERO_PS();
    simd_vec_ambient_color.y = SET_ZERO_PS();
    simd_vec_ambient_color.z = SET_ZERO_PS();

    simd_vec_ambient_color.x = MULT_PS(simd_vec_mat_surfCol.x, SET1_PS(MATERIAL_AMBIENT_COEFF));
    simd_vec_ambient_color.y = MULT_PS(simd_vec_mat_surfCol.y, SET1_PS(MATERIAL_AMBIENT_COEFF));
    simd_vec_ambient_color.z = MULT_PS(simd_vec_mat_surfCol.z, SET1_PS(MATERIAL_AMBIENT_COEFF));
    // END 1 

    // START 2 - Compute Specular color
    SIMD_VEC_PS simd_vec_specular_color;
    simd_vec_specular_color.x = SET1_PS(SPECULAR_COEFF);
    simd_vec_specular_color.y = SET1_PS(SPECULAR_COEFF);
    simd_vec_specular_color.z = SET1_PS(SPECULAR_COEFF);

    simd_vec_specular_color.x = MULT_PS(simd_vec_specular_color.x, simd_mmd_specular);
    simd_vec_specular_color.y = MULT_PS(simd_vec_specular_color.y, simd_mmd_specular);
    simd_vec_specular_color.z = MULT_PS(simd_vec_specular_color.z, simd_mmd_specular);

    simd_vec_specular_color.x = MULT_PS(simd_vec_specular_color.x, sdf_shadow_info.s); // TODO: replace 1.0 by s
    simd_vec_specular_color.y = MULT_PS(simd_vec_specular_color.y, sdf_shadow_info.s); // for soft shadows
    simd_vec_specular_color.z = MULT_PS(simd_vec_specular_color.z, sdf_shadow_info.s);

    simd_vec_specular_color.x = MULT_PS(simd_vec_specular_color.x, scene->light->emissionColor->x);
    simd_vec_specular_color.y = MULT_PS(simd_vec_specular_color.y, scene->light->emissionColor->y);
    simd_vec_specular_color.z = MULT_PS(simd_vec_specular_color.z, scene->light->emissionColor->z);

    simd_vec_specular_color.x = MULT_PS(simd_vec_specular_color.x, simd_mmd_light_inv_dist);
    simd_vec_specular_color.y = MULT_PS(simd_vec_specular_color.y, simd_mmd_light_inv_dist);
    simd_vec_specular_color.z = MULT_PS(simd_vec_specular_color.z, simd_mmd_light_inv_dist);
    // END 2

    simd_vec_finalColor->x = ADD_PS(ADD_PS(ADD_PS(simd_vec_diffuse_color.x, simd_vec_specular_color.x), simd_vec_ambient_color.x), simd_vec_finalColor->x);
    simd_vec_finalColor->y = ADD_PS(ADD_PS(ADD_PS(simd_vec_diffuse_color.y, simd_vec_specular_color.y), simd_vec_ambient_color.y), simd_vec_finalColor->y);
    simd_vec_finalColor->z = ADD_PS(ADD_PS(ADD_PS(simd_vec_diffuse_color.z, simd_vec_specular_color.z), simd_vec_ambient_color.z), simd_vec_finalColor->z);

#if FOG == 1

    SIMD_MMS simd_mmd_t;
    simd_vec_norm(&sdf_info.intersection_pt, &simd_mmd_t);

    alignas(32) float t[NR_SIMD_VEC_ELEMS]; // TODO: implement exp func instead of doing this
    STORE_PS(t, simd_mmd_t);

    SIMD_MMS simd_mmd_fog_val = SET_PS(exp(FOG_COEFF * t[7] * t[7] * t[7]), exp(FOG_COEFF * t[6] * t[6] * t[6]), 
                                       exp(FOG_COEFF * t[5] * t[5] * t[5]), exp(FOG_COEFF * t[4] * t[4] * t[4]), 
                                       exp(FOG_COEFF * t[3] * t[3] * t[3]), exp(FOG_COEFF * t[2] * t[2] * t[2]), 
                                       exp(FOG_COEFF * t[1] * t[1] * t[1]), exp(FOG_COEFF * t[0] * t[0] * t[0]));

    // SIMD_MMS simd_mmd_fog_coeff = SET1_PS(FOG_COEFF);
    // SIMD_MMS simd_mmd_fog_t = SET_PS(t[7], t[6], t[5], t[4], t[3], t[2], t[1], t[0]);
    // SIMD_MMS simd_mmd_fog_val;
    // simd_mmd_fog_val  = MULT_PS(simd_mmd_fog_coeff, MULT_PS(simd_mmd_fog_t, MULT_PS(simd_mmd_fog_t, simd_mmd_fog_t)));
    // simd_mmd_fog_val = exp2f8(simd_mmd_fog_val);

    simd_vec_finalColor->x = MULT_PS(simd_vec_finalColor->x, simd_mmd_fog_val);
    simd_vec_finalColor->y = MULT_PS(simd_vec_finalColor->y, simd_mmd_fog_val);
    simd_vec_finalColor->z = MULT_PS(simd_vec_finalColor->z, simd_mmd_fog_val);
#endif

    // mask out
    simd_vec_finalColor->x = AND_PS(simd_vec_finalColor->x, sdf_info.intersected_mask);
    simd_vec_finalColor->y = AND_PS(simd_vec_finalColor->y, sdf_info.intersected_mask);
    simd_vec_finalColor->z = AND_PS(simd_vec_finalColor->z, sdf_info.intersected_mask);
}


/*
 * Shoot 8 packed floating-point rays and 
 * trace them through the scene.
 */
FORCE_INLINE void shoot_rays_and_trace(Scene* scene, 
                                       unsigned int x, 
                                       unsigned int y,
                                       float px_col_x_out[NR_SIMD_VEC_ELEMS],
                                       float px_col_y_out[NR_SIMD_VEC_ELEMS],
                                       float px_col_z_out[NR_SIMD_VEC_ELEMS])
{
#if AA > 1
            set_zero(tot_col);
            for (int m = 0; m < AA; m++)
            {
                for (int n = 0; n < AA; n++)
                {
                    // pixel coordinates
                    float disp_x = (inv_AA * n - 0.5) + x;
                    float disp_y = (inv_AA * m - 0.5) + y;
                    shoot_ray(scene->camera, disp_x, disp_y, dir);
                    trace(scene->camera->pos, dir, &scene, 0, px_col);
                    vec_add(tot_col, px_col, tot_col);
                }
            }
            float px_col[NR_VEC_ELEMENTS];
            vec_mult_scalar(tot_col, inv_AA2, px_col);
#else

            SIMD_VEC_PS simd_vec_dir;
            shoot_rays(scene->camera, x, y, &simd_vec_dir);

            // Copy 4 times origin
            SIMD_VEC_PS simd_vec_orig;
            simd_vec_orig.x = SET1_PS(scene->camera->pos[0]);
            simd_vec_orig.y = SET1_PS(scene->camera->pos[1]);
            simd_vec_orig.z = SET1_PS(scene->camera->pos[2]);

            // Init color 
            SIMD_VEC_PS simd_vec_finalColor;
            simd_vec_finalColor.x = SET_ZERO_PS(); 
            simd_vec_finalColor.y = SET_ZERO_PS();
            simd_vec_finalColor.z = SET_ZERO_PS();

            trace(&simd_vec_orig, &simd_vec_dir, scene, 0, &simd_vec_finalColor);

#if GAMMA_CORR == 1
            
            SIMD_MMS _const_pd = SET1_PS(0.4545);
            simd_mmd_pow_func(&simd_vec_finalColor.x, &_const_pd, &simd_vec_finalColor.x);
            simd_mmd_pow_func(&simd_vec_finalColor.y, &_const_pd, &simd_vec_finalColor.y);
            simd_mmd_pow_func(&simd_vec_finalColor.z, &_const_pd, &simd_vec_finalColor.z);
#endif

            SIMD_MMS _m_255 = SET1_PS(255);
            SIMD_MMS _min_color_x = MIN_PS(SET1_PS(1.0), simd_vec_finalColor.x);
            SIMD_MMS _min_color_y = MIN_PS(SET1_PS(1.0), simd_vec_finalColor.y);
            SIMD_MMS _min_color_z = MIN_PS(SET1_PS(1.0), simd_vec_finalColor.z);

            SIMD_MMS color_x = MULT_PS(_min_color_x, _m_255);
            SIMD_MMS color_y = MULT_PS(_min_color_y, _m_255);
            SIMD_MMS color_z = MULT_PS(_min_color_z, _m_255);

            STORE_PS(px_col_x_out, color_x);
            STORE_PS(px_col_y_out, color_y);
            STORE_PS(px_col_z_out, color_z);

#endif

}


/*
 * Function: render
 * ----------------------------
 *   Render an image based on the defined scene and save it in ouput. 
 *
 *   sps: scene, spheres
 *   pLight: point light 
 *
 *   returns: void
 */
void render(Scene* scene)
{
#if AA > 1
    float inv_AA = 1.0 / AA;
    float inv_AA2 = inv_AA / AA;
    float tot_col[NR_VEC_ELEMENTS];
#endif

    int width = scene->camera->widthPx;
    int height = scene->camera->heightPx;

    int rest_width = width % NR_SIMD_VEC_ELEMS;
    int mult_width = width - rest_width;

    alignas(32) float px_col_x[NR_SIMD_VEC_ELEMS];
    alignas(32) float px_col_y[NR_SIMD_VEC_ELEMS];
    alignas(32) float px_col_z[NR_SIMD_VEC_ELEMS];

    for (unsigned y = 0; y < height; ++y)
    {
        int y_w_4 = y * width * 4;

        // NORMAL LOOP
        unsigned x = 0;
        for (; x < mult_width; x += NR_SIMD_VEC_ELEMS) 
        {
            shoot_rays_and_trace(scene, x, y, px_col_x, px_col_y, px_col_z);

            // save colors computed by trace into current pixel
            scene->img[y_w_4 + x * 4 + 0] = (unsigned char)(px_col_x[0]);
            scene->img[y_w_4 + x * 4 + 1] = (unsigned char)(px_col_y[0]);
            scene->img[y_w_4 + x * 4 + 2] = (unsigned char)(px_col_z[0]);
            scene->img[y_w_4 + x * 4 + 3] = 255;

            scene->img[y_w_4 + (x+1) * 4 + 0] = (unsigned char)(px_col_x[1]);
            scene->img[y_w_4 + (x+1) * 4 + 1] = (unsigned char)(px_col_y[1]);
            scene->img[y_w_4 + (x+1) * 4 + 2] = (unsigned char)(px_col_z[1]);
            scene->img[y_w_4 + (x+1) * 4 + 3] = 255;

            scene->img[y_w_4 + (x+2) * 4 + 0] = (unsigned char)(px_col_x[2]);
            scene->img[y_w_4 + (x+2) * 4 + 1] = (unsigned char)(px_col_y[2]);
            scene->img[y_w_4 + (x+2) * 4 + 2] = (unsigned char)(px_col_z[2]);
            scene->img[y_w_4 + (x+2) * 4 + 3] = 255;

            scene->img[y_w_4 + (x+3) * 4 + 0] = (unsigned char)(px_col_x[3]);
            scene->img[y_w_4 + (x+3) * 4 + 1] = (unsigned char)(px_col_y[3]);
            scene->img[y_w_4 + (x+3) * 4 + 2] = (unsigned char)(px_col_z[3]);
            scene->img[y_w_4 + (x+3) * 4 + 3] = 255;

            scene->img[y_w_4 + (x+4) * 4 + 0] = (unsigned char)(px_col_x[4]);
            scene->img[y_w_4 + (x+4) * 4 + 1] = (unsigned char)(px_col_y[4]);
            scene->img[y_w_4 + (x+4) * 4 + 2] = (unsigned char)(px_col_z[4]);
            scene->img[y_w_4 + (x+4) * 4 + 3] = 255;

            scene->img[y_w_4 + (x+5) * 4 + 0] = (unsigned char)(px_col_x[5]);
            scene->img[y_w_4 + (x+5) * 4 + 1] = (unsigned char)(px_col_y[5]);
            scene->img[y_w_4 + (x+5) * 4 + 2] = (unsigned char)(px_col_z[5]);
            scene->img[y_w_4 + (x+5) * 4 + 3] = 255;

            scene->img[y_w_4 + (x+6) * 4 + 0] = (unsigned char)(px_col_x[6]);
            scene->img[y_w_4 + (x+6) * 4 + 1] = (unsigned char)(px_col_y[6]);
            scene->img[y_w_4 + (x+6) * 4 + 2] = (unsigned char)(px_col_z[6]);
            scene->img[y_w_4 + (x+6) * 4 + 3] = 255;

            scene->img[y_w_4 + (x+7) * 4 + 0] = (unsigned char)(px_col_x[7]);
            scene->img[y_w_4 + (x+7) * 4 + 1] = (unsigned char)(px_col_y[7]);
            scene->img[y_w_4 + (x+7) * 4 + 2] = (unsigned char)(px_col_z[7]);
            scene->img[y_w_4 + (x+7) * 4 + 3] = 255;
        }

        // RESIDUAL LOOP (if width is not multiple of 4)
        if (x < width) 
        {
            shoot_rays_and_trace(scene, mult_width, y, px_col_x, px_col_y, px_col_z);

            // save colors computed by trace into current pixel
            for (size_t i = 0; i < rest_width; i++)
            {
                scene->img[y_w_4 + (mult_width + i) * 4 + 0] = (unsigned char)(px_col_x[i]);
                scene->img[y_w_4 + (mult_width + i) * 4 + 1] = (unsigned char)(px_col_y[i]);
                scene->img[y_w_4 + (mult_width + i) * 4 + 2] = (unsigned char)(px_col_z[i]);
                scene->img[y_w_4 + (mult_width + i) * 4 + 3] = 255;
            }
            
            

        }

    }
}


/*
 * Function: render_all
 * ----------------------------
 *   Render all the different scenes and save it in ouput. 
 *
 *   returns: void
 */
void render_all(SceneContainer scenes_container)
{

    for (int i = 0; i < scenes_container.num_scenes; ++i)
    {
        // Get scene and malloc output image
        Scene* s = (scenes_container.scenes)[i];
        create_image(s, s->camera->widthPx, s->camera->heightPx);
        
        // Render
        render(s);
        save_image_to_disk(s, NULL);

        // Clean
        destroy_scene(s);
        destroy_image(s);
        free((scenes_container.scenes)[i]);
    }
    free(scenes_container.scenes);
}

int main(int argc, char **argv)
{

    // Compute Global Constants
    log_base_e = log2f8(SET1_PS(2.71828182845904523536));
    log_base_e = DIV_PS(SET1_PS(1.0), log_base_e);

    //--- Check if output directory exists, create otherwise ---//
    struct stat st = {0};
    if (stat(OUTPUT_PATH, &st) == -1)
    {
        mkdir(OUTPUT_PATH, 0700); // create directory
    }

    //--- Read arguments and create scenes ---//
    SceneContainer scenes_container;
    if (argc == 1)
    {
        printf("Add scene scene0 into index 0.\n");
        scenes_container = create_scene_container(1);
        add_scene(&scenes_container, "scene0", 0);
    }
    else
    {   
        if (strcmp(argv[1], "benchmark") == 0)
        {
            RUN_STATE = M_BENCHMARK;
            if (argc == 2)
            {
                printf("Add scene scene0 into index 0.\n");
                scenes_container = create_scene_container(1);
                add_scene(&scenes_container, "scene0", 0);
            }
            else
            {
                scenes_container = create_scene_container(argc - 2);
            }

            for (int i = 2; i < argc; i++)
            {
                printf("Add scene %s into index %d.\n", argv[i], i - 2);
                add_scene(&scenes_container, argv[i], i - 2);
            }
            
        }
        else
        {
            scenes_container = create_scene_container(argc - 1);
            for (int i = 1; i < argc; i++)
            {
                printf("Add scene %s into index %d.\n", argv[i], i - 1);
                add_scene(&scenes_container, argv[i], i - 1);
            }
        }
    }

    //--- Run benchmarking or rendering ---//
    if (RUN_STATE == M_BENCHMARK)
    {
        //--- BENCHMARKING ---//
        /// TODO:
        benchmark_render(&render, scenes_container);
        // benchmark_trace(&trace, scenes_container);
    }
    else
    {
        // Check if render_out directory exists, create otherwise
        if (stat(RENDER_OUT, &st) == -1)
        {
            mkdir(RENDER_OUT, 0700); // create directory
        }

        //--- RENDER ALL ---//
        render_all(scenes_container);
    }

    return 0;
}
