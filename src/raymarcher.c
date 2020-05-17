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
const double const_specularColour[3] = {SPECULAR_COEFF, SPECULAR_COEFF, SPECULAR_COEFF};
const double const_eps_x[3] = {EPSILON_NORMALS, 0, 0};
const double const_eps_y[3] = {0, EPSILON_NORMALS, 0};
const double const_eps_z[3] = {0, 0, EPSILON_NORMALS};

enum Mode
{
    M_RENDER = 0, 
    M_BENCHMARK = 1
}; 

static enum Mode RUN_STATE = M_RENDER;

//===============================
//            CODE
//===============================
void compute_normal(const SIMD_VEC* simd_vec_p, const Scene *scene, SIMD_VEC* simd_vec_normals_out)
{
    // TODO: preallocate this
    SIMD_MMD SIMD_MMD_EPSILON = SET1_PD(EPSILON_NORMALS);

    SIMD_VEC simd_vec_p0;
    simd_vec_p0.x = ADD_PD(simd_vec_p->x, SIMD_MMD_EPSILON);
    simd_vec_p0.y = simd_vec_p->y;
    simd_vec_p0.z = simd_vec_p->z;

    SIMD_VEC simd_vec_p1;
    simd_vec_p1.x = simd_vec_p->x;
    simd_vec_p1.y = ADD_PD(simd_vec_p->y, SIMD_MMD_EPSILON);
    simd_vec_p1.z = simd_vec_p->z;

    SIMD_VEC simd_vec_p2;
    simd_vec_p2.x = simd_vec_p->x; 
    simd_vec_p2.y = simd_vec_p->y;
    simd_vec_p2.z = ADD_PD(simd_vec_p->z, SIMD_MMD_EPSILON);

    // SIMD_VEC for min_dist of normal points
    SIMD_VEC simd_vec_min_dist_p;
    
    // SIMD_VEC min_dist of shifted points
    SIMD_VEC simd_vec_min_dist_p_eps;

    SDF_Info sdf_info;
    sdf_info.finish_ray_mask = SET1_PD(0.0);
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

    SIMD_VEC sub_res;
    simd_vec_sub(&simd_vec_min_dist_p_eps, &simd_vec_min_dist_p, &sub_res);

    simd_vec_normalize(&sub_res, simd_vec_normals_out);
}

double compute_specular_coefficient(const double dir[NR_VEC_ELEMENTS], double N[NR_VEC_ELEMENTS], double L[NR_VEC_ELEMENTS], Material *mat)
{
    // Light reflected on normal
    double __R[NR_VEC_ELEMENTS];
    double __V[NR_VEC_ELEMENTS];
    vec_mult_scalar(L, -1, __V);
    vec_reflect(__V, N, __R);
    vec_mult_scalar(dir, -1, __V);
    vec_normalize(__V);

    // Specular term
    double specAngle = max(vec_dot(__R, __V), 0.0);
    return pow(specAngle, (*mat).shininess);
}

// void compute_shadow_coefficient(SDF_Info *sdf_info, double *ph, double *t)
// {
//     double mid = sdf_info->min_dist * sdf_info->min_dist;
//     double y = mid / (2.0 * (*ph));
//     double d = sqrt(mid - y * y);
//     sdf_info->s = min(sdf_info->s, LIGHT_STR * d / max(0.0, (*t) - y));
//     *ph = sdf_info->min_dist;
//     *t += sdf_info->min_dist;
// }

void ray_march(SIMD_VEC *simd_vec_orig, SIMD_VEC *simd_vec_dir, const Scene *scene, const int doShadowSteps, SDF_Info* sdf_info_out)
{
    sdf_info_out->finish_ray_mask_int = 0;
    sdf_info_out->finish_ray_mask = SET1_PD(0.0);
    sdf_info_out->intersected_mask = SET1_PD(0.0);

    SIMD_MMD t = SET1_PD(EPSILON);
    SIMD_MMD ph = SET1_PD(1e20);
    sdf_info_out->s = SET1_PD(1.0);

    SIMD_MMD simd_mmd_THRESHOLD = SET1_PD(INTERSECT_THRESHOLD);
    SIMD_MMD simd_mmd_BBOX_AXES = SET1_PD(BBOX_AXES);

    SIMD_MMD intersection_mask;
    int current_int_intersection_mask = 0;

    SIMD_MMD simd_mmd_norm_sq;
    SIMD_MMD overshoot_mask;
    SIMD_MMD current_overshoot_mask = SET1_PD(0.0);
    int current_int_overshoot_mask = 0;

    SIMD_MMD min_dist_for_marching; // has 0 for points that finished

    // Create marching point
    SIMD_VEC simd_vec_march_pt = *simd_vec_orig;

    for (int i = 0; i < MARCH_COUNT; ++i)
    {
        // Populate sdf_info.min_dist with min distances for the 4 rays
        sdf(&simd_vec_march_pt, scene, sdf_info_out);

        // Compute intersection mask
        // debug_simd_mmd(&sdf_info.min_dist);
        intersection_mask = CMP_PD(sdf_info_out->min_dist, simd_mmd_THRESHOLD, _CMP_LT_OS);
        int int_intersection_mask = MOVEMASK_PD(intersection_mask);
        
        // Don't march with intersected or overshooted rays
        sdf_info_out->finish_ray_mask = OR_PD(sdf_info_out->intersected_mask, current_overshoot_mask);
        min_dist_for_marching = ANDNOT_PD(sdf_info_out->finish_ray_mask, sdf_info_out->min_dist);

        // March
        simd_vec_march_pt.x = ADD_PD(simd_vec_march_pt.x, MULT_PD(simd_vec_dir->x, min_dist_for_marching));
        simd_vec_march_pt.y = ADD_PD(simd_vec_march_pt.y, MULT_PD(simd_vec_dir->y, min_dist_for_marching));
        simd_vec_march_pt.z = ADD_PD(simd_vec_march_pt.z, MULT_PD(simd_vec_dir->z, min_dist_for_marching));

        // EXIT CONDITIONS [TODO: can move before march to precompute final_mask | but we do it like this now like we always did]
        // Check intersection
        if (current_int_intersection_mask != int_intersection_mask)
        {
            current_int_intersection_mask = int_intersection_mask;
            sdf_info_out->intersected_mask = intersection_mask;

            // printf("\n\n current_int_intersection_mask is: %d", current_int_intersection_mask);
            if (current_int_intersection_mask == 0b1111)
            {
                // Save intersected ray (min_dist and nearest_obj_idx should already be right)
                sdf_info_out->intersection_pt = simd_vec_march_pt;
                break;
            }
        }

        // Check overshoot
        simd_vec_norm_squared(&simd_vec_march_pt, &simd_mmd_norm_sq);
        overshoot_mask = CMP_PD(simd_mmd_norm_sq, simd_mmd_BBOX_AXES, _CMP_GT_OS);
        int int_overshoot_mask = MOVEMASK_PD(overshoot_mask);
        if (current_int_overshoot_mask != int_overshoot_mask)
        {
            current_overshoot_mask = overshoot_mask;
            current_int_overshoot_mask = int_overshoot_mask;

            if (current_int_overshoot_mask == 0b1111)
            {
                // Save intersected ray (min_dist and nearest_obj_idx should already be right)
                sdf_info_out->intersection_pt = simd_vec_march_pt;
                break;
            }
        }

        // Check combination
        // If all ray in state intersected or overshooted -> exit
        sdf_info_out->finish_ray_mask_int = MOVEMASK_PD(sdf_info_out->finish_ray_mask);
        if (sdf_info_out->finish_ray_mask_int == 0b1111)
        {
            sdf_info_out->intersection_pt = simd_vec_march_pt;
            break;
        }
    }
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
void trace(SIMD_VEC *simd_vec_orig,
           SIMD_VEC *simd_vec_dir,
           const Scene *scene,
           const int depth, 
           double vec_res_finalColor[NR_VEC_ELEMENTS])
{
    // GLOBAL VARIABLES
    double v__ambientColor[NR_VEC_ELEMENTS]; 
    // set zero
    v__ambientColor[0] = 0.0;
    v__ambientColor[1] = 0.0;
    v__ambientColor[2] = 0.0;
    vec_res_finalColor[0] = 0.0;
    vec_res_finalColor[1] = 0.0;
    vec_res_finalColor[2] = 0.0;
    double v__specularColor[NR_VEC_ELEMENTS];
    v__specularColor[0] = const_specularColour[0];
    v__specularColor[1] = const_specularColour[1];
    v__specularColor[2] = const_specularColour[2];
    double v__diffuseColor[NR_VEC_ELEMENTS];
    // double v__N[NR_VEC_ELEMENTS]; // Normal direction 
    double v__L[NR_VEC_ELEMENTS]; // Light direction
    double v__tmp_res[NR_VEC_ELEMENTS]; // temporary result variable
    double v__vmult[NR_VEC_ELEMENTS]; // temporary multiplication variable
    SDF_Info sdf_shadow_info, sdf_info;
    Material mat;
    double lambertian = 0;
    double specular = 0;

    // debug_simd_vec(simd_vec_dir);
    // debug_simd_vec(simd_vec_orig);

    double vec_dir[NR_VEC_ELEMENTS];
    alignas(32) double vec_dir_x[NR_SIMD_VEC_ELEMS];
    alignas(32) double vec_dir_y[NR_SIMD_VEC_ELEMS];
    alignas(32) double vec_dir_z[NR_SIMD_VEC_ELEMS];
    STORE_PD(vec_dir_x, simd_vec_dir->x);
    STORE_PD(vec_dir_y, simd_vec_dir->y);
    STORE_PD(vec_dir_z, simd_vec_dir->z);
    vec_dir[0] = vec_dir_x[0];
    vec_dir[1] = vec_dir_y[0];
    vec_dir[2] = vec_dir_z[0];

    // CHECK INTERSECTION WITH SCENE
    ray_march(simd_vec_orig, simd_vec_dir, scene, 0, &sdf_info);

    // ============================================
    // REST OF FUNCTION IS SCALAR IN ORDER TO DEBUG
    // ============================================
    alignas(32) double intersected[NR_SIMD_VEC_ELEMS];
    STORE_PD(intersected, sdf_info.intersected_mask);

    alignas(32) double nearest_obj_idx[NR_SIMD_VEC_ELEMS];
    STORE_PD(nearest_obj_idx, sdf_info.nearest_obj_idx);


    alignas(32) double inter_pt_x[NR_SIMD_VEC_ELEMS];
    alignas(32) double inter_pt_y[NR_SIMD_VEC_ELEMS];
    alignas(32) double inter_pt_z[NR_SIMD_VEC_ELEMS];
    STORE_PD(inter_pt_x, sdf_info.intersection_pt.x);
    STORE_PD(inter_pt_y, sdf_info.intersection_pt.y);
    STORE_PD(inter_pt_z, sdf_info.intersection_pt.z);
    double intersection_pt[NR_VEC_ELEMENTS];
    intersection_pt[0] = inter_pt_x[0];
    intersection_pt[1] = inter_pt_y[0];
    intersection_pt[2] = inter_pt_z[0];

    // ============================================
    // END DEBUG
    // ============================================

    // No intersection case (return black)
    if (intersected[0] == 0)
        return;

    // Shade intersected object TODO: remove hardcoded part
    mat = *(scene->geometric_ojects[(int)nearest_obj_idx[0]]->mat); //*(scene->geometric_ojects[sdf_info.nearest_obj_idx]->mat);

    // Normal
    SIMD_VEC simd_vec_normals;
    compute_normal(&sdf_info.intersection_pt, scene, &simd_vec_normals);

    //=============================================
    // DEBUG: get normal to a vector to check if it's right
    //=============================================
    alignas(32) double v__N_x[NR_SIMD_VEC_ELEMS];
    alignas(32) double v__N_y[NR_SIMD_VEC_ELEMS];
    alignas(32) double v__N_z[NR_SIMD_VEC_ELEMS];
    STORE_PD(v__N_x, simd_vec_normals.x);
    STORE_PD(v__N_y, simd_vec_normals.y);
    STORE_PD(v__N_z, simd_vec_normals.z);
    double v__N[NR_VEC_ELEMENTS];
    v__N[0] = v__N_x[0];
    v__N[1] = v__N_y[0];
    v__N[2] = v__N_z[0];
    //=============================================
    //=============================================

    // In theory not necessary if normals are computed outwards
    if (vec_dot(vec_dir, v__N) > 0)
    {
        vec_mult_scalar(v__N, -1, v__N);
    }

    // TODO: REFLECTION
    // if ((mat.refl > 0) && (depth < MAX_RAY_DEPTH))
    // {
    //     double v__reflDir[NR_VEC_ELEMENTS];
    //     double v__reflectedCol[NR_VEC_ELEMENTS];

    //     // Compute reflected dir
    //     vec_reflect(vec_dir, v__N, v__reflDir);
    //     vec_normalize(v__reflDir);

    //     // Compute reflected color
    //     vec_mult_scalar(v__N, EPSILON, v__tmp_res);
    //     vec_add(sdf_info.intersection_pt, v__tmp_res, v__tmp_res);

    //     trace(v__tmp_res, v__reflDir, scene, depth + 1, v__reflectedCol);

    //     vec_mult_scalar(v__reflectedCol, mat.refl, v__tmp_res);
    //     vec_mult_scalar(v__tmp_res, REFLECTIVE_COEFF, vec_res_finalColor);
    // }

    // Light dir L
    vec_sub(scene->light->c, intersection_pt, v__L);

    // distance between intersection_pt and light source
    double dist = vec_norm(v__L);
    double inv_dist = 1 / dist;
    vec_mult_scalar(v__L, inv_dist, v__L);

    // TODO: SHADOWS
    // sdf_shadow_info.s = 1.0;
    // if (scene->nr_geom_objs > 1)
    // {
    //     vec_mult_scalar(v__L, EPSILON, v__tmp_res);
    //     vec_add(sdf_info.intersection_pt, v__tmp_res, v__tmp_res);
    //     sdf_shadow_info = ray_march(v__tmp_res, v__L, scene, 1);
    //     sdf_shadow_info.s = clamp(sdf_shadow_info.s, SHADOW_LIGHTNESS, 1.0);
    // }

    // Lamber's cosine law
    lambertian = max(vec_dot(v__N, v__L), 0.0);
    if (lambertian > 0.0)
    {
        specular = compute_specular_coefficient(vec_dir, v__N, v__L, &mat);
    }

    // Diffuse Colour Computation
    vec_mult_scalar(mat.surfCol, lambertian, v__tmp_res);
    vec_mult_scalar(v__tmp_res, 1.0, v__tmp_res);
    vec_mult(scene->light->emissionColor, v__tmp_res, v__vmult);
    vec_mult_scalar(v__vmult, inv_dist, v__diffuseColor); // diffuse colour result

    // Ambient Colour Computation
    vec_mult_scalar(mat.surfCol, MATERIAL_AMBIENT_COEFF, v__tmp_res);
    vec_add(v__ambientColor, v__tmp_res, v__ambientColor); // ambient colour result
    
    // Specular Colour Computation
    vec_mult_scalar(v__specularColor, specular, v__tmp_res);
    vec_mult(scene->light->emissionColor, v__tmp_res, v__vmult);
    vec_mult_scalar(v__vmult, 1.0, v__tmp_res);
    vec_mult_scalar(v__tmp_res, inv_dist, v__specularColor); // specular colour result

    // Final Colour 
    vec_add(v__ambientColor, v__diffuseColor, v__tmp_res);
    vec_add(v__tmp_res, v__specularColor, v__tmp_res);
    vec_add(vec_res_finalColor, v__tmp_res, vec_res_finalColor); // final colour result

#if FOG == 1
    double t = vec_norm(intersection_pt);
    vec_mult_scalar(vec_res_finalColor, exp(FOG_COEFF * t * t * t), vec_res_finalColor);
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
#if DEBUG_MODE == 1
    printf("%s", "RENDERING... ");
    float progress = 0.;
    float progress_step = 1. / (scene.camera->widthPx * scene.camera->heightPx);
    progress += progress_step;
#endif

#if AA > 1
    double inv_AA = 1.0 / AA;
    double inv_AA2 = inv_AA / AA;
    double tot_col[NR_VEC_ELEMENTS];
#endif


    int width = scene->camera->widthPx;
    int height = scene->camera->heightPx;

    // double dir[NR_VEC_ELEMENTS]; 
    double px_col[NR_VEC_ELEMENTS];

    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; x++) // TODO: unroll by 4
        {

#if AA > 1
            set_zero(tot_col);
            for (int m = 0; m < AA; m++)
            {
                for (int n = 0; n < AA; n++)
                {
                    // pixel coordinates
                    double disp_x = (inv_AA * n - 0.5) + x;
                    double disp_y = (inv_AA * m - 0.5) + y;
                    shoot_ray(scene->camera, disp_x, disp_y, dir);
                    trace(scene->camera->pos, dir, &scene, 0, px_col);
                    vec_add(tot_col, px_col, tot_col);
                }
            }
            double px_col[NR_VEC_ELEMENTS];
            vec_mult_scalar(tot_col, inv_AA2, px_col);
#else

            double dir0[NR_VEC_ELEMENTS]; 
            double dir1[NR_VEC_ELEMENTS]; 
            double dir2[NR_VEC_ELEMENTS]; 
            double dir3[NR_VEC_ELEMENTS]; 

            shoot_ray(scene->camera, x, y, dir0); 
            shoot_ray(scene->camera, x+1, y, dir1); 
            shoot_ray(scene->camera, x+2, y, dir2); 
            shoot_ray(scene->camera, x+3, y, dir3); 

            // create simd vec
            alignas(32) double dir_x[NR_SIMD_VEC_ELEMS]; 
            alignas(32) double dir_y[NR_SIMD_VEC_ELEMS]; 
            alignas(32) double dir_z[NR_SIMD_VEC_ELEMS]; 
            create_vec_x(dir0, dir1, dir2, dir3, dir_x);
            create_vec_y(dir0, dir1, dir2, dir3, dir_y);
            create_vec_z(dir0, dir1, dir2, dir3, dir_z);

            SIMD_VEC simd_vec_dir;
            simd_vec_dir.x = LOAD_PD(dir_x);
            simd_vec_dir.y = LOAD_PD(dir_y);
            simd_vec_dir.z = LOAD_PD(dir_z);

            // Copy 4 times origin
            SIMD_VEC simd_vec_orig;
            simd_vec_orig.x = SET1_PD(scene->camera->pos[0]);
            simd_vec_orig.y = SET1_PD(scene->camera->pos[1]);
            simd_vec_orig.z = SET1_PD(scene->camera->pos[2]);

            trace(&simd_vec_orig, &simd_vec_dir, scene, 0, px_col);

#endif

#if GAMMA_CORR == 1
            vec_pow_inplace(px_col, 0.4545);
#endif
            // save colors computed by trace into current pixel
            scene->img[y * width * 4 + x * 4 + 0] = (unsigned char)(min(1, px_col[0]) * 255);
            scene->img[y * width * 4 + x * 4 + 1] = (unsigned char)(min(1, px_col[1]) * 255);
            scene->img[y * width * 4 + x * 4 + 2] = (unsigned char)(min(1, px_col[2]) * 255);
            scene->img[y * width * 4 + x * 4 + 3] = 255;

#if DEBUG_MODE == 1
            progress += progress_step;
            fflush(stdout);
            printf(" %.2f\b\b\b\b\b", progress);
            fflush(stdout);
#endif
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
        printf("Add scene shape0 into index 0.\n");
        scenes_container = create_scene_container(1);
        add_scene(&scenes_container, "shape0", 0);
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
