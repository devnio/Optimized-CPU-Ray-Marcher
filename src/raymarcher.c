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

#include "geometry/scene.h"
#include "lodepng.h"
#include "camera.h"
#include "utility.h"
#include "light.h"
#include "scene_loader.h"

// ===== MACROS =====

// RENDERING
#define MAX_RAY_DEPTH 2    // max nr. bounces
#define MARCH_COUNT 3000   // max marching steps
#define BBOX_AXES 100     // bounding box size
#define INTERSECT_THRESHOLD 0.00001 // careful with this -> should be low enoguh for shadow to work

// SHADING
#define SPECULAR_COEFF 0.2
#define SHADOW_LIGHTNESS 0.1
#define LIGHT_STR 3

#define FOG 1
#define FOG_COEFF -0.00002

// PRECISION
#define EPSILON 0.001

// RESOLUTION
#define WIDTH 1280
#define HEIGHT 720

// ANTI ALIASING
#define AA 1

// GAMMA CORRECTION
#define GAMMA_CORR 1

// DEBUG
#define DEBUG_MODE 1

Vec3 compute_normal(Vec3 p, Scene scene)
{
    Vec3 p0 = vec_add(p, new_vector(EPSILON, 0, 0));
    Vec3 p1 = vec_add(p, new_vector(0, EPSILON, 0));
    Vec3 p2 = vec_add(p, new_vector(0, 0, EPSILON));
    
    SDF_Info sdf_info;
    sdf(p, scene, &sdf_info);
    Vec3 c = new_vector_one(sdf_info.min_dist); 
    
    Vec3 ch;
    sdf(p0, scene, &sdf_info); ch.x = sdf_info.min_dist;
    sdf(p1, scene, &sdf_info); ch.y = sdf_info.min_dist;
    sdf(p2, scene, &sdf_info); ch.z = sdf_info.min_dist;
    
    // Vec3 n = vec_mult_scalar(vec_sub(ch, c), 1.0/EPSILON);
    // return n;
    Vec3 n = vec_sub(ch, c);
    return vec_normalized(n);
}

SDF_Info ray_march(Vec3 p, Vec3 dir, Scene scene, int doShadowSteps)
{
    SDF_Info sdf_info;
    Vec3 march_pt = p;

    double t = EPSILON;
    double ph = 1e20;
    sdf_info.s = 1.0;

    for (int i = 0; i < MARCH_COUNT; i++)
    {
        sdf(march_pt, scene, &sdf_info);
        march_pt = vec_add(march_pt, vec_mult_scalar(dir, sdf_info.min_dist));

        // TOL
        if (sdf_info.min_dist < INTERSECT_THRESHOLD)
        {
            sdf_info.intersected = 1;
            sdf_info.intersection_pt = march_pt;
            break;
        }
        // BBOX CHECK
        if (vec_norm(march_pt) > BBOX_AXES)
        {
            sdf_info.intersected = 0;
            break;
        }

        if (doShadowSteps==1)
        {
            float mid = sdf_info.min_dist*sdf_info.min_dist;
            double y = mid/(2.0*ph); 
            double d = sqrt(mid-y*y);
            sdf_info.s = min(sdf_info.s, LIGHT_STR*d/max(0.0,t-y));
            ph = sdf_info.min_dist;
            t += sdf_info.min_dist;
        }
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
Vec3 trace(Vec3 o, 
           Vec3 dir, 
           Scene scene, 
           PointLight pLight, 
           int depth)
{
    // SOME GLOBAL VARIABLES
    Vec3 ambientColor = new_vector(0, 0, 0);
    Vec3 finalColor = new_vector(0, 0, 0);

    // CHECK INTERSECTION WITH SCENE
    SDF_Info sdf_info = ray_march(o, dir, scene, 0);

    // No intersection case (return black)
    if (sdf_info.intersected != 1)
        return ambientColor;


    // Shade intersected object    
    Material mat = *(scene.geometric_ojects[sdf_info.nearest_obj_idx]->mat);

    // Normal
    Vec3 N = compute_normal(sdf_info.intersection_pt, scene);

    // In theory not necessary if normals are computed outwards
    if (vec_dot(dir, N) > 0)
    {
        N = vec_mult_scalar(N, -1);
    }

    // Light dir
    Vec3 L = vec_sub(pLight.c, sdf_info.intersection_pt);
    L = vec_normalized(L);

    if ((depth < MAX_RAY_DEPTH) && (mat.refl > 0))
    {
        // Compute reflected dir
        Vec3 reflDir = vec_sub(dir, vec_mult_scalar(vec_mult_scalar(N, vec_dot(dir, N)), 2));
        reflDir = vec_normalized(reflDir);

        // Compute reflected color
        Vec3 reflectedCol = trace(vec_add(sdf_info.intersection_pt, vec_mult_scalar(N, EPSILON)), reflDir, scene, pLight, depth + 1);
        finalColor = vec_mult_scalar(reflectedCol, mat.refl);
    }

    /* Before doing anything else check if shadow ray.
     * We assume that light is not in between objects. 
     * Otherwise should check only interval between light and sdf_info.intersection_pt. 
    */
   SDF_Info sdf_shadow_info;
   sdf_shadow_info.s = 1.0;
   if (scene.nr_geom_objs > 1)
   {
        sdf_shadow_info = ray_march(vec_add(sdf_info.intersection_pt, vec_mult_scalar(L, EPSILON)), L, scene, 1);
        sdf_shadow_info.s = clamp(sdf_shadow_info.s, SHADOW_LIGHTNESS, 1.0);
        // if (sdf_shadow_info.intersected == 1) return new_vector(0.0, 0.0, 0.0);
   }

    // Lamber's cosine law
    double lambertian = max(vec_dot(N, L), 0.0);
    double specular = 0;
    if (lambertian > 0.0)
    {
        // Light reflected on normal
        Vec3 R = vec_reflect(vec_mult_scalar(L, -1), N);
        Vec3 V = vec_normalized(vec_mult_scalar(dir, -1));

        // Specular term
        double specAngle = max(vec_dot(R, V), 0.0);
        specular = pow(specAngle, mat.shininess);
    }

    Vec3 diffuseColor = vec_mult_scalar(vec_mult_scalar(mat.surfCol, lambertian), sdf_shadow_info.s);
    Vec3 specularColor = new_vector(SPECULAR_COEFF, SPECULAR_COEFF, SPECULAR_COEFF);
    specularColor = vec_mult(pLight.emissionColor, vec_mult_scalar(specularColor, specular));
    finalColor = vec_add(finalColor, vec_add(vec_add(ambientColor, diffuseColor), specularColor));

#if FOG == 1
    double t = vec_norm(sdf_info.intersection_pt);
    finalColor = vec_mult_scalar(finalColor, exp(FOG_COEFF * t * t * t));
#endif

    return finalColor;
}

void encodeOneStep(const char *filename, const unsigned char *image, unsigned width, unsigned height)
{
    /*Encode the image*/
    unsigned error = lodepng_encode32_file(filename, image, width, height);

    /*if there's an error, display it*/
    if (error)
        printf("error %u: %s\n", error, lodepng_error_text(error));
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
void render(Scene scene, PointLight pLight)
{
    unsigned int width = WIDTH;
    unsigned int height = HEIGHT;
    float fov = 30;

    Camera* camera = create_camera(fov, width, height);

    //Translation and rotation
    // Vec3 t = {0.0,5.0,-7};
    // move_camera(camera, t);
    rotate_camera(camera, 5, 0);

    // debug
    printf("RENDERING... ");
    float progress = 0.;
    float progress_step = 1./(WIDTH*HEIGHT);
    progress += progress_step;

    double inv_AA = 1.0/AA;
    Vec3 tot_col;

    // render
    size_t png_img_size = width * height * 4 * sizeof(unsigned char);
    unsigned char *img = (unsigned char *)malloc(png_img_size);
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {

#if AA>1
            tot_col = new_vector(0,0,0);
            for( int m=0; m<AA; m++ )
            {
                for( int n=0; n<AA; n++ )
                {
                    // pixel coordinates
                    double disp_x = (inv_AA*n - 0.5) + x;
                    double disp_y = (inv_AA*m - 0.5) + y;
                    Vec3 dir = shoot_ray(camera, disp_x, disp_y);
                    Vec3 px_col = trace(camera->pos, dir, scene, pLight, 0, NULL);
                    tot_col = vec_add(tot_col, px_col);
                }
            }
            Vec3 px_col = vec_mult_scalar(tot_col, 1.0/(AA*AA));
#else
            Vec3 dir = shoot_ray(camera, x, y);
            Vec3 px_col = trace(camera->pos, dir, scene, pLight, 0);

                
#endif
            
#if GAMMA_CORR == 1
            px_col = vec_pow( px_col, 0.4545 );
#endif
            // save colors computed by trace into current pixel
            img[y * width * 4 + x * 4 + 0] = (unsigned char)(min(1, px_col.x) * 255);
            img[y * width * 4 + x * 4 + 1] = (unsigned char)(min(1, px_col.y) * 255);
            img[y * width * 4 + x * 4 + 2] = (unsigned char)(min(1, px_col.z) * 255);
            img[y * width * 4 + x * 4 + 3] = 255;

#if DEBUG_MODE == 1
            
            progress += progress_step;
            fflush(stdout);
            printf(" %.2f\b\b\b\b\b", progress);
            fflush(stdout);
#endif
            

        }
    }

    encodeOneStep(scene.name, img, width, height);
    printf("\nImage rendered and saved in output folder\n");
    free(img);
    free_camera(camera);
    
}

/*
 * Function: render_all
 * ----------------------------
 *   Render all the different scenes and save it in ouput. 
 *
 *   pLight: point light 
 *
 *   returns: void
 */
void render_all(SceneContainer scenes_container, PointLight pLight){

    // for(int i=0;i<scenes_container.num_scenes;++i){
        Scene s = *(scenes_container.scenes)[0];
        // printf("Scene check 1... entry 3: %f",s.geometric_ojects[0]->params[3]);
        render(s, pLight);
        destroy_scene(&s);
    // }
    free((scenes_container.scenes)[0]);
}

int main()
{
    SceneContainer scenes_container = build_scenes();
 
    // Lights (in future can be an array)
    PointLight pLight;
    pLight.c = new_vector(0, 100, 0);
    double em = 2;
    pLight.emissionColor = new_vector(em, em, em);

    //render(*(scenes_container.scenes)[1], pLight);
    render_all(scenes_container, pLight);

    return 0;
}


