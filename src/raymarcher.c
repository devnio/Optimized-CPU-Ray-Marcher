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
void compute_normal(double vec_p[NR_VEC_ELEMENTS], const Scene *scene, double res[NR_VEC_ELEMENTS])
{
    SIMD_VEC simd_vec_p;
    simd_vec_p.x = SET1_PD(vec_p[0]);
    simd_vec_p.y = SET1_PD(vec_p[1]);
    simd_vec_p.z = SET1_PD(vec_p[2]);

    double v__ch[NR_VEC_ELEMENTS];
    double v__c[NR_VEC_ELEMENTS];
    // double v__p0[NR_VEC_ELEMENTS];
    // double v__p1[NR_VEC_ELEMENTS];
    // double v__p2[NR_VEC_ELEMENTS];

    SIMD_VEC simd_vec_p0;
    simd_vec_p0.x = SET1_PD(vec_p[0] + EPSILON_NORMALS);
    simd_vec_p0.y = SET1_PD(vec_p[1]);
    simd_vec_p0.z = SET1_PD(vec_p[2]);
    SIMD_VEC simd_vec_p1;
    simd_vec_p1.x = SET1_PD(vec_p[0]);
    simd_vec_p1.y = SET1_PD(vec_p[1] + EPSILON_NORMALS);
    simd_vec_p1.z = SET1_PD(vec_p[2]);
    SIMD_VEC simd_vec_p2;
    simd_vec_p2.x = SET1_PD(vec_p[0]);
    simd_vec_p2.y = SET1_PD(vec_p[1]);
    simd_vec_p2.z = SET1_PD(vec_p[2] + EPSILON_NORMALS);

    // vec_add(vec_p, const_eps_x, v__p0);
    // vec_add(vec_p, const_eps_y, v__p1);
    // vec_add(vec_p, const_eps_z, v__p2);

    SDF_Info sdf_info;
    sdf(&simd_vec_p, scene, &sdf_info);


    // ============================
    // DEBUG
    // ============================
    alignas(32) double min_d[NR_SIMD_VEC_ELEMS]; STORE_PD(min_d, sdf_info.min_dist);
    set_vec_from_double(v__c, min_d[0]); // TODO: make work for 4

    sdf(&simd_vec_p0, scene, &sdf_info);
    STORE_PD(min_d, sdf_info.min_dist);
    v__ch[0] = min_d[0];


    sdf(&simd_vec_p1, scene, &sdf_info);
    STORE_PD(min_d, sdf_info.min_dist);
    v__ch[1] = min_d[0];


    sdf(&simd_vec_p2, scene, &sdf_info);
    STORE_PD(min_d, sdf_info.min_dist);
    v__ch[2] = min_d[0];

    vec_sub(v__ch, v__c, res);
    vec_normalize(res);
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

SDF_Info ray_march(SIMD_VEC *simd_vec_orig, SIMD_VEC *simd_vec_dir, const Scene *scene, const int doShadowSteps)
{
    SDF_Info sdf_info;
    sdf_info.intersected = SET1_PD(0.0);

    SIMD_MMD simd_mmd_THRESHOLD = SET1_PD(INTERSECT_THRESHOLD);

    // copy origin point -> we will be changing this point (maybe we can skip this if no problem arises)
    SIMD_VEC simd_vec_march_pt = *simd_vec_orig;

    SIMD_MMD t = SET1_PD(EPSILON);
    SIMD_MMD ph = SET1_PD(1e20);
    // sdf_info.s = 1.0; // TODO: soft shadows later

    // double v__tmp[NR_VEC_ELEMENTS];


    for (int i = 0; i < MARCH_COUNT; ++i)
    {
        sdf(&simd_vec_march_pt, scene, &sdf_info);

        // March
        simd_vec_march_pt.x = ADD_PD(simd_vec_march_pt.x, MULT_PD(simd_vec_dir->x, sdf_info.min_dist));
        simd_vec_march_pt.y = ADD_PD(simd_vec_march_pt.y, MULT_PD(simd_vec_dir->y, sdf_info.min_dist));
        simd_vec_march_pt.z = ADD_PD(simd_vec_march_pt.z, MULT_PD(simd_vec_dir->z, sdf_info.min_dist));
        
        // =======================================
        // DEBUG
        // =======================================
        alignas(32) double out_x[NR_SIMD_VEC_ELEMS]; STORE_PD(out_x, simd_vec_march_pt.x);
        alignas(32) double out_y[NR_SIMD_VEC_ELEMS]; STORE_PD(out_y, simd_vec_march_pt.y);
        alignas(32) double out_z[NR_SIMD_VEC_ELEMS]; STORE_PD(out_z, simd_vec_march_pt.z);
        double march_pt[NR_VEC_ELEMENTS];
        march_pt[0] = out_x[0];
        march_pt[1] = out_y[0];
        march_pt[2] = out_z[0]; 


        alignas(32) double min_dist[NR_SIMD_VEC_ELEMS]; STORE_PD(min_dist, sdf_info.min_dist);
        // printf("i=%d, MARCH POINT: %f %f %f | MIN DIST: %f", i, march_pt[0], march_pt[1],  march_pt[2], min_dist[0]);

        // TOL
        if (min_dist[0] < INTERSECT_THRESHOLD) // TODO: make work for 4
        {
            sdf_info.intersected = SET1_PD(1.0);
            sdf_info.intersection_pt[0] = march_pt[0];
            sdf_info.intersection_pt[1] = march_pt[1];
            sdf_info.intersection_pt[2] = march_pt[2];
            break;
        }
        
        // BBOX CHECK
        if (vec_norm_squared(march_pt) > BBOX_AXES)
        {
            sdf_info.intersected = SET1_PD(0.0);
            break;
        }

        // if (doShadowSteps == 1)
        // {
        //     compute_shadow_coefficient(&sdf_info, &ph, &t);
        // }
    }

    return sdf_info;
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
    double v__N[NR_VEC_ELEMENTS]; // Normal direction 
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
    sdf_info = ray_march(simd_vec_orig, simd_vec_dir, scene, 0);

    // ============================================
    // REST OF FUNCTION IS SCALAR IN ORDER TO DEBUG
    // ============================================
    alignas(32) double intersected[NR_SIMD_VEC_ELEMS];
    STORE_PD(intersected, sdf_info.intersected);

    // ============================================
    // END DEBUG
    // ============================================

    // No intersection case (return black)
    if (intersected[0] != 1)
        return;

    // Shade intersected object TODO: remove hardcoded part
    mat =  *(scene->geometric_ojects[0]->mat); //*(scene->geometric_ojects[sdf_info.nearest_obj_idx]->mat);

    // Normal
    compute_normal(sdf_info.intersection_pt, scene, v__N);

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
    vec_sub(scene->light->c, sdf_info.intersection_pt, v__L);

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
    double t = vec_norm(sdf_info.intersection_pt);
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
