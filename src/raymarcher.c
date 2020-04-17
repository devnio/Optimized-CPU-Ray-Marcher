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

// ===== MACROS =====

// RENDERING
#define MAX_RAY_DEPTH 2    // max nr. bounces
#define MARCH_COUNT 3000   // max marching steps
#define BBOX_AXES 100     // bounding box size

#define SPECULAR_COEFF 0.2

#define EPSILON 0.001
#define LIGHT_STR 1.5

// SCREEN
#define WIDTH 1280
#define HEIGHT 720

// ANTI ALIASING
#define AA 1

// DEBUG
#define DEBUG_MODE 1

Vec3 compute_normal(Vec3 p, Scene scene)
{
    Vec3 p0 = vec_add(p, new_vector(EPSILON, 0, 0));
    Vec3 p1 = vec_add(p, new_vector(0, EPSILON, 0));
    Vec3 p2 = vec_add(p, new_vector(0, 0, EPSILON));
    
    Vec3 c = new_vector_one(sdf(p, scene, NULL).min_dist); 
    Vec3 ch;
    ch.x = sdf(p0, scene, NULL).min_dist;
    ch.y = sdf(p1, scene, NULL).min_dist;
    ch.z = sdf(p2, scene, NULL).min_dist;
    
    // Vec3 n = vec_mult_scalar(vec_sub(ch, c), 1.0/EPSILON);
    // return n;
    Vec3 n = vec_sub(ch, c);
    return vec_normalized(n);
}

SDF_Info ray_march(Vec3 p, Vec3 dir, Scene scene, SDF_Info* prev_sdf_info, int doShadowSteps)
{
    SDF_Info sdf_info;
    Vec3 march_pt = p;

    double t = 0;
    double s = 1.0;
    float ph = 1e10;

    for (int i = 0; i < MARCH_COUNT; i++)
    {
        sdf_info = sdf(march_pt, scene, prev_sdf_info);
        march_pt = vec_add(march_pt, vec_mult_scalar(dir, sdf_info.min_dist));

        // TOL
        if (sdf_info.min_dist < 0.0001)
        {
            //printf("\n ENTERED \n");
            sdf_info.intersected = 1;
            sdf_info.intersection_pt = march_pt;
            break;
        }
        // BBOX CHECK
        if (vec_norm(march_pt) > BBOX_AXES)
        {
            break;
        }

        if (doShadowSteps==1)
        {
            float mid = sdf_info.min_dist*sdf_info.min_dist;
            float y = mid/(2.0*ph);
            float d = sqrt(mid-y*y);
            s = min(s, LIGHT_STR*d/max(0.0,t-y));
            ph = sdf_info.min_dist;
            t += sdf_info.min_dist;
        }
    }
    
    sdf_info.s = s;
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
           int depth, 
           SDF_Info *prev_sdf_info)
{
    // SOME GLOBAL VARIABLES
    Vec3 ambientColor = new_vector(0, 0, 0);
    Vec3 finalColor = new_vector(0, 0, 0);

    // CHECK INTERSECTION WITH SCENE
    SDF_Info sdf_info = ray_march(o, dir, scene, prev_sdf_info, 0);

    // No intersection case (return black)
    if (sdf_info.intersected != 1)
        return ambientColor;


    // Shade intersected object    
    Material mat;

    // Find the object and take it's material
    if (sdf_info.nearest_obj_type == T_Plane)
    {
        mat = scene.planes[sdf_info.nearest_obj_idx].mat;
    }
    else if (sdf_info.nearest_obj_type == T_Cone)
    {
        mat = scene.cones[sdf_info.nearest_obj_idx].mat;
    }

    else if (sdf_info.nearest_obj_type == T_Octahedron)
    {
        mat = scene.octahedrons[sdf_info.nearest_obj_idx].mat;
    }
    else if (sdf_info.nearest_obj_type == T_Sphere)
    {
        mat = scene.spheres[sdf_info.nearest_obj_idx].mat;
    } 
    else if (sdf_info.nearest_obj_type == T_Box)
    {
        mat = scene.boxes[sdf_info.nearest_obj_idx].mat;
    }

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
        double bias = 1e-4;
        Vec3 reflectedCol = trace(vec_add(sdf_info.intersection_pt, vec_mult_scalar(N, bias)), reflDir, scene, pLight, depth + 1, &sdf_info);
        finalColor = vec_mult_scalar(reflectedCol, mat.refl);
    }

    /* Before doing anything else check if shadow ray.
     * We assume that light is not in between objects. 
     * Otherwise should check only interval between light and sdf_info.intersection_pt. 
    */
    SDF_Info sdf_shadow_info = ray_march(sdf_info.intersection_pt, L, scene, &sdf_info, 1);
    sdf_shadow_info.s = clamp(sdf_shadow_info.s+0.5, 0, 1);

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
    float invWidth = 1. / (float)width;
    float invHeight = 1. / (float)height;
    float fov = 15;
    float aspectratio = width / (float)height;
    float angle = tan(M_PI * 0.5 * fov / 180.);

    Camera* camera = create_camera(fov, width, height, 0, 1000);

    //Translation and rotation
    Vec3 t = {0.0,0.0,-10};
    move_camera(camera, t);
    rotate_camera(camera, -3, 0);

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
            Vec3 px_col = trace(camera->pos, dir, scene, pLight, 0, NULL);
#endif
            
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
    printf("\nImage rendered and saved in output folder\n");
    free(img);
    free_camera(camera);
}

int main()
{
    // planes
    Plane pl0;
    pl0.n = new_vector(0,1,0);
    pl0.d = 3;
    pl0.mat.surfCol = new_vector(0, 0.3, 0.6);
    pl0.mat.refl = 0;
    pl0.mat.shininess = 15;
    pl0.mat.emissionColor = new_vector(0, 0, 0);

    // Ocathedron
    Octahedron oct0;
    oct0.c = new_vector(4, 0, 20);
    oct0.s = 2;
    oct0.mat.surfCol = new_vector(0, 0.3, 0.6);
    oct0.mat.refl = 0;
    oct0.mat.shininess = 15;
    oct0.mat.emissionColor = new_vector(0, 0, 0);

    // spheres
    Sphere sp0;
    sp0.c = new_vector(50, 0, 100);
    sp0.r = 50;
    sp0.r2 = 2500;
    sp0.mat.surfCol = new_vector(0.6, 0.6, 0);
    sp0.mat.refl = 0;
    sp0.mat.shininess = 15;
    sp0.mat.emissionColor = new_vector(0, 0, 0);
    
    Sphere sp1;
    sp1.c = new_vector(4, 0, 25);
    sp1.r = 3;
    sp1.r2 = 9;
    sp1.mat.surfCol = new_vector(0.8, 0.1, 0);
    sp1.mat.refl = 0.3;
    sp1.mat.shininess = 5;
    sp1.mat.emissionColor = new_vector(0, 0, 0);
    
    Sphere sp2;
    sp2.c = new_vector(-4, 0, 15);
    sp2.r = 3;
    sp2.r2 = 9;
    sp2.mat.surfCol = new_vector(0.3, 1, 0.36);
    sp2.mat.refl = 0;
    sp2.mat.shininess = 105;
    sp2.mat.emissionColor = new_vector(0, 0, 0);
    
    Sphere sp3;
    sp3.c = new_vector(0, 0, 40);
    sp3.r = 3;
    sp3.r2 = 9;
    sp3.mat.surfCol = new_vector(0.2, 0.2, 0.97);
    sp3.mat.refl = 0.1;
    sp3.mat.shininess = 15;
    sp3.mat.emissionColor = new_vector(0, 0, 0);

    // box
    Box box0;
    Vec3 vec_box0; vec_box0.x = 1.0; vec_box0.y = 1.0; vec_box0.z = 1.0;
    box0.b = vec_box0; 
    box0.mat.surfCol = new_vector(0.3, 1, 0.36);
    box0.mat.refl = 0;
    box0.mat.shininess = 105;
    box0.mat.emissionColor = new_vector(0, 0, 0);

    // ================
    // cones
    Cone cone0;
    cone0.c = new_vector(4, 3, 25);
    cone0.r1 = 3;
    cone0.r2 = 0.01;
    cone0.h = 3;
    cone0.mat.surfCol = new_vector(0.8, 0.1, 0);
    cone0.mat.refl = 0.1;
    cone0.mat.shininess = 5;
    cone0.mat.emissionColor = new_vector(0, 0, 0);

    Cone cone1;
    cone1.c = new_vector(-4, 3, 15);
    cone1.r1 = 3;
    cone1.r2 = 0.01;
    cone1.h = 3;
    cone1.mat.surfCol = new_vector(0.3, 1, 0.36);
    cone1.mat.refl = 0.1;
    cone1.mat.shininess = 105;
    cone1.mat.emissionColor = new_vector(0, 0, 0);


    Cone cone2;
    cone2.c = new_vector(0, 3, 40);
    cone2.r1 = 3;
    cone2.r2 = 0.01;
    cone2.h = 3;
    cone2.mat.surfCol = new_vector(0.2, 0.2, 0.97);
    cone2.mat.refl = 0.1;
    cone2.mat.shininess = 15;
    cone2.mat.emissionColor = new_vector(0, 0, 0);

    // scene definition
    // ================
    Scene scene;
    scene.nr_planes = 1;
    scene.nr_spheres = 4;
    scene.nr_boxes = 1;
    scene.nr_cones = 3;
    scene.nr_octahedrons = 1;

    // build plane array
    Plane planes[scene.nr_planes];
    planes[0] = pl0;

    // build Ocathedron array
    Octahedron octahedrons[scene.nr_octahedrons];
    octahedrons[0] = oct0;

    // build sphere array
    Sphere sps[scene.nr_spheres];
    sps[0] = sp0;
    sps[1] = sp1;
    sps[2] = sp2;
    sps[3] = sp3;
    
    // build pyramid array
    Box boxes[scene.nr_boxes];
    boxes[0] = box0;

    // build cone array
    Cone cones[scene.nr_cones];
    cones[0] = cone0;
    cones[1] = cone1;
    cones[2] = cone2;

    // assign arrays to scene
    scene.planes = planes;
    scene.spheres = sps;
    scene.boxes = boxes;
    scene.cones = cones;
    scene.octahedrons = octahedrons;

    // Lights (in future can be an array)
    PointLight pLight;
    pLight.c = new_vector(0, 100, 0);
    double em = 2;
    pLight.emissionColor = new_vector(em, em, em);

    render(scene, pLight);
    return 0;
}
