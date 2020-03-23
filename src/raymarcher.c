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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "lodepng.h"
#include "camera.h"
#include "utility.h"
#include "sphere.h"
#include "light.h"

// MACROS
#define MAX_RAY_DEPTH 4
#define OBJS_IN_SCENE 5
#define SPECULAR_COEFF 0.2

// Screen width and height
#define WIDTH 1280
#define HEIGHT 720



/*
 * Function: trace
 * ----------------------------
 *   TODO: function describtion 
 *
 *   TODO
 *
 *   returns: TODO
 */
Vec3 trace(Vec3 o, Vec3 dir, Sphere sps[], PointLight pLight, int depth, int excludeSp)
{
    // SOME GLOBAL VARIABLES
    Vec3 ambientColor = new_vector(0, 0, 0);
    Vec3 finalColor = new_vector(0, 0, 0);
    float specular = 0;

    // CHECK INTERSECTION WITH SCENE
    double minDist = INFINITY;
    Vec3 phit = o;
    Sphere *nearestSp = NULL;
    int currIdx;
    for (int i = 0; i < 1000; i++)
    {
        for (int k = 0; k < OBJS_IN_SCENE; k++)
        {
            if (k == excludeSp)
                continue;

            double dist = sdf(phit, sps[k]);
            if (dist < minDist)
            {
                minDist = dist;
                currIdx = k;
            }
        }
        phit = vec_add(phit, vec_mult_scalar(dir, minDist));
        // TOL
        if (minDist < 0.0001)
        {
            nearestSp = &sps[currIdx];
            break;
        }
    }

    // No intersection case (return black)
    if (!nearestSp)
        return new_vector(0, 0, 0);
    float bias = 1e-4;
    Vec3 surfaceColor = nearestSp->surfCol;

    // Hit point
    // Vec3 phit = vec_add(o, vec_mult1(dir, tnear));
    // Normal
    Vec3 N = vec_sub(phit, nearestSp->c);
    N = vec_normalized(N);

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
        Vec3 reflectedCol = trace(vec_add(phit, vec_mult_scalar(N, bias)), reflDir, sps, pLight, depth + 1, currIdx);
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
 *   TODO: function describtion 
 *
 *   TODO
 *
 *   returns: void
 */
void render(Sphere sps[], PointLight pLight)
{
    unsigned int width = WIDTH;
    unsigned int height = HEIGHT;
    float invWidth = 1 / (float)width;
    float invHeight = 1 / (float)height;
    float fov = 30;
    float aspectratio = width / (float)height;
    float angle = tan(M_PI * 0.5 * fov / 180.);


    Camera* camera = create_camera(fov, width, height, 0, 1000);

    //translation example
    Vec3 t = {0.0,0.0,0.0};
    move_camera(camera, t);

    // TRACE
    size_t png_img_size = width * height * 4 * sizeof(unsigned char);
    unsigned char *img = (unsigned char *)malloc(png_img_size);
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            Vec3 dir = shoot_ray(camera, x, y);

            Vec3 px_col = trace(new_vector(0, 0, 0), dir, sps, pLight, 0, -1);
            px_col = px_col;

            img[y * width * 4 + x * 4 + 0] = (unsigned char)(min(1, px_col.x) * 255);
            img[y * width * 4 + x * 4 + 1] = (unsigned char)(min(1, px_col.y) * 255);
            img[y * width * 4 + x * 4 + 2] = (unsigned char)(min(1, px_col.z) * 255);
            img[y * width * 4 + x * 4 + 3] = 255;
        }
    }

    encodeOneStep("../output/output_img.png", img, width, height);
    printf("Image rendered and saved in output folder");
    free(img);
    free_camera(camera);
}

int main()
{
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
    pLight.c = new_vector(0, 550, 15);
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
