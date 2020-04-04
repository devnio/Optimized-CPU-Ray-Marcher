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

#include "lodepng.h"
#include "camera.h"
#include "utility.h"
#include "sphere.h"
#include "light.h"

// ===== MACROS =====

// RENDERING
#define MAX_RAY_DEPTH 2    // max nr. bounces
#define MARCH_COUNT 5000   // max marching steps
#define BBOX_AXES 100     // bounding box size

#define OBJS_IN_SCENE 5
#define SPECULAR_COEFF 0.2

#define EPSILON 0.0001

// SCREEN
#define WIDTH 1280
#define HEIGHT 720

// DEBUG
#define DEBUG_MODE 1

typedef struct  
{
    double min_dist;
    int intersected_obj_idx;
} SDF_Info;

// TODO: make more general for whole scene and not only spheres
// This is marching for the entire scene returning the sdf
SDF_Info sdf(Vec3 p, Sphere sps[], int excludeSp)
{
    SDF_Info sdf_info;
    double minDist = INFINITY;
    int currIdx;
    for (int k = 0; k < OBJS_IN_SCENE; k++)
    {
        if (k == excludeSp)
            continue;

        double dist = sdf_sphere(p, sps[k]);
        if (dist < minDist)
        {
            minDist = dist;
            currIdx = k;
        }
    }

    sdf_info.min_dist = minDist;
    sdf_info.intersected_obj_idx = currIdx;
    return sdf_info;
}

Vec3 compute_normal(Vec3 p, Sphere sps[])
{
    Vec3 p0 = vec_add(p, new_vector(EPSILON, 0, 0));
    Vec3 p1 = vec_add(p, new_vector(0, EPSILON, 0));
    Vec3 p2 = vec_add(p, new_vector(0, 0, EPSILON));
    
    Vec3 c = new_vector_one(sdf(p, sps, -1).min_dist); 
    Vec3 ch;
    ch.x = sdf(p0, sps, -1).min_dist;
    ch.y = sdf(p1, sps, -1).min_dist;
    ch.z = sdf(p2, sps, -1).min_dist;
    
    Vec3 n = vec_sub(ch, c);
    return vec_normalized(n);
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
           Sphere sps[], 
           PointLight pLight, 
           int depth, 
           int excludeSp)
{
    // SOME GLOBAL VARIABLES
    Vec3 ambientColor = new_vector(0, 0, 0);
    Vec3 finalColor = new_vector(0, 0, 0);
    float specular = 0;

    // CHECK INTERSECTION WITH SCENE
    Vec3 phit = o;
    Sphere *nearestSp = NULL;
    SDF_Info sdf_info;

    for (int i = 0; i < MARCH_COUNT; i++)
    {
        sdf_info = sdf(phit, sps, excludeSp);
        phit = vec_add(phit, vec_mult_scalar(dir, sdf_info.min_dist));
        // TOL
        if (sdf_info.min_dist < 0.0001)
        {
            nearestSp = &sps[sdf_info.intersected_obj_idx];
            break;
        }
        // BBOX CHECK
        if (vec_norm(phit) > BBOX_AXES)
        {
            break;
        }
    }

    // No intersection case (return black)
    if (!nearestSp)
        return new_vector(0, 0, 0);
    float bias = 1e-4;
    Vec3 surfaceColor = nearestSp->surfCol;

    // Normal
    // Vec3 N = vec_sub(phit, nearestSp->c);
    // N = vec_normalized(N);
    Vec3 N = compute_normal(phit, sps);

    // In theory not necessary if normals are computed outwards
    if (vec_dot(dir, N) > 0)
    {
        N = vec_mult_scalar(N, -1);
    }

    // Light dir
    Vec3 L = vec_sub(pLight.c, phit);
    L = vec_normalized(L);

    if ((depth < MAX_RAY_DEPTH) && (nearestSp->refl > 0))
    {
        // Compute reflected dir
        Vec3 reflDir = vec_sub(dir, vec_mult_scalar(vec_mult_scalar(N, vec_dot(dir, N)), 2));
        reflDir = vec_normalized(reflDir);

        // Compute reflected color
        Vec3 reflectedCol = trace(vec_add(phit, vec_mult_scalar(N, bias)), reflDir, sps, pLight, depth + 1, sdf_info.intersected_obj_idx);
        finalColor = vec_mult_scalar(reflectedCol, nearestSp->refl);
    }

    // Before doing anything else check if shadow ray
    for (int j = 0; j < OBJS_IN_SCENE; j++)
    {
        float t_shadow = sphere_ray_intersection(vec_add(phit, vec_mult_scalar(N, bias)), L, sps[j]);
        if (t_shadow > 0)
        {
            return ambientColor; // return vec_add(finalColor, ambientColor);
        }
    }

    // Lamber's cosine law
    float lambertian = max(vec_dot(N, L), 0.0);
    if (lambertian > 0.0)
    {
        // Light reflected on normal
        Vec3 R = vec_reflect(vec_mult_scalar(L, -1), N);
        Vec3 V = vec_normalized(vec_mult_scalar(dir, -1));

        // Specular term
        float specAngle = max(vec_dot(R, V), 0.0);
        specular = pow(specAngle, nearestSp->shininess);
    }

    Vec3 diffuseColor = vec_mult_scalar(surfaceColor, lambertian);
    Vec3 specularColor = new_vector(SPECULAR_COEFF, SPECULAR_COEFF, SPECULAR_COEFF);
    specularColor = vec_mult(pLight.emissionColor, vec_mult_scalar(specularColor, specular));
    finalColor = vec_add(finalColor, vec_add(vec_add(ambientColor, diffuseColor), specularColor));

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
void render(Sphere sps[], PointLight pLight)
{
    unsigned int width = WIDTH;
    unsigned int height = HEIGHT;
    float invWidth = 1. / (float)width;
    float invHeight = 1. / (float)height;
    float fov = 30;
    float aspectratio = width / (float)height;
    float angle = tan(M_PI * 0.5 * fov / 180.);

    Camera* camera = create_camera(fov, width, height, 0, 1000);

    // translation example
    Vec3 t = {0.0,0.0,0.0};
    move_camera(camera, t);

    // debug
    printf("RENDERING... ");
    float progress = 0.;
    float progress_step = 1./(WIDTH*HEIGHT);

    // render
    size_t png_img_size = width * height * 4 * sizeof(unsigned char);
    unsigned char *img = (unsigned char *)malloc(png_img_size);
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            Vec3 dir = shoot_ray(camera, x, y);

            Vec3 px_col = trace(new_vector(0, 0, 0), dir, sps, pLight, 0, -1);
            px_col = px_col;

            // save colors computed by trace into current pixel
            img[y * width * 4 + x * 4 + 0] = (unsigned char)(min(1, px_col.x) * 255);
            img[y * width * 4 + x * 4 + 1] = (unsigned char)(min(1, px_col.y) * 255);
            img[y * width * 4 + x * 4 + 2] = (unsigned char)(min(1, px_col.z) * 255);
            img[y * width * 4 + x * 4 + 3] = 255;

            if (DEBUG_MODE)
            {
                progress += progress_step;
                fflush(stdout);
                printf(" %.2f\b\b\b\b\b", progress);
                fflush(stdout);
            }
        }
    }

    encodeOneStep("../output/output_img.png", img, width, height);
    printf("Image rendered and saved in output folder");
    free(img);
    free_camera(camera);
}

int main()
{
    // scene definition
    Sphere sp0;
    sp0.c = new_vector(0, -10004, 20);
    sp0.r = 10000;
    sp0.r2 = 100000000;
    sp0.surfCol = new_vector(0.2, 0.3, 0.8);
    sp0.refl = 0;
    sp0.shininess = 15;
    sp0.emissionColor = new_vector(0, 0, 0);
    Sphere sp1;
    sp1.c = new_vector(4, 0, 25);
    sp1.r = 3;
    sp1.r2 = 9;
    sp1.surfCol = new_vector(0.8, 0, 0);
    sp1.refl = 0.5;
    sp1.shininess = 15;
    sp1.emissionColor = new_vector(0, 0, 0);
    Sphere sp2;
    sp2.c = new_vector(-4, 0, 15);
    sp2.r = 3;
    sp2.r2 = 9;
    sp2.surfCol = new_vector(0.3, 1, 0.36);
    sp2.refl = 0;
    sp2.shininess = 15;
    sp2.emissionColor = new_vector(0, 0, 0);
    Sphere sp3;
    sp3.c = new_vector(0, 0, 40);
    sp3.r = 3;
    sp3.r2 = 9;
    sp3.surfCol = new_vector(0.2, 0.2, 0.97);
    sp3.refl = 0.1;
    sp3.shininess = 15;
    sp3.emissionColor = new_vector(0, 0, 0);
    Sphere sp4;
    sp4.c = new_vector(50, 0, 100);
    sp4.r = 50;
    sp4.r2 = 2500;
    sp4.surfCol = new_vector(0.6, 0.6, 0);
    sp4.refl = 0;
    sp4.shininess = 15;
    sp4.emissionColor = new_vector(0, 0, 0);

    // Lights (in future can be an array)
    PointLight pLight;
    pLight.c = new_vector(0, 100, 0);
    double em = 2;
    pLight.emissionColor = new_vector(em, em, em);

    Sphere sps[OBJS_IN_SCENE];
    sps[0] = sp0;
    sps[1] = sp1;
    sps[2] = sp2;
    sps[3] = sp3;
    sps[4] = sp4;

    render(sps, pLight);
    return 0;
}
