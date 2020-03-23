#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "camera.h"

// MACROS
#define MAX_RAY_DEPTH 4
#define OBJS_IN_SCENE 4

/* ===============
 * Sphere - START
 * =============== */
typedef struct 
{
    Vec3 c;
    double r;
    double r2;
    Vec3 surfCol;
    double refl;
    Vec3 emissionColor;
    // for now leave out emission, transparency, reflectance
} Sphere;

double sphere_ray_intersection(Vec3 o, Vec3 dir, Sphere s)
{
    Vec3 l = vec_sub(s.c, o);
    float tca = vec_dot(l, dir);
    if (tca < 0) return -1;

    float d2 = vec_dot(l, l) - tca * tca;
    if (d2 > s.r2) return -1;

    float thc = sqrt(s.r2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;
    
    if (t0 < 0) return t1;
    else return t0;
    // check if t0 < 0
}

/* ===============
 * Sphere - END
 * =============== */

/* ===============
 * HELPER - START
 * =============== */
double mix(double a, double b, double mix)
{
    return b * mix + a * (1 - mix);
}

double min(double a, double b)
{
    if (a < b) return a;
    else return b;
}

double max(double a, double b)
{
    if (a < b) return b;
    else return a;
}

Vec3 color_addWeighted(Vec3 colA, Vec3 colB, double weightA, double weightB) {
    double sum = weightA + weightB;
    Vec3 c;
    c.x = ((colA.x * weightA) + (colB.x * weightB)) / sum;
    c.y = ((colA.y * weightA) + (colB.y * weightB)) / sum;
    c.z = ((colA.z * weightA) + (colB.z * weightB)) / sum;
    return c;
}

Vec3 color_blend(Vec3 colA, Vec3 colB, double weightA)
{
    double wA = weightA > 1 ? 1 : weightA;
    double weightB = 1 - wA;
    Vec3 c;
    return color_addWeighted(colA, colB, wA, weightB);
}
/* ===============
 * HELPER - END
 * =============== */


Vec3 trace(Vec3 o, Vec3 dir, Sphere sps[], int depth)
{
    // CHECK INTERSECTION WITH SCENE
    float tnear = INFINITY;
    Sphere *nearestSp = NULL;
    for (int i = 0; i < OBJS_IN_SCENE; i++)
    {
        double t = sphere_ray_intersection(o, dir, sps[i]);
        if (t > 0)
        {
            if (t < tnear)
            {
                tnear = t;
                nearestSp = &sps[i];
            }
        }
    }
    
    // No intersection case (return black)
    if (!nearestSp) return new_vector(0, 0, 0);
    
    Vec3 surfaceColor = nearestSp->surfCol;
    Vec3 phit = vec_add(o, vec_mult1(dir, tnear));
    Vec3 nhit = vec_sub(phit, nearestSp->c);
    nhit = vec_normalized(nhit);

    float bias = 1e-4; // add some bias to the point from which we will be tracing
    int inside = 0;
    if (vec_dot(dir, nhit) > 0) 
    {
        nhit = vec_mult1(nhit, -1);
        inside = 1; // used for transparency
    }

    if ((depth < MAX_RAY_DEPTH) && (nearestSp->refl > 0)) 
    {
        // COMPUTE reflection value
        Vec3 reflDir = vec_sub(dir, vec_mult1(vec_mult1(nhit, vec_dot(dir, nhit)), 2));
        reflDir = vec_normalized(reflDir);

        Vec3 reflection = trace(vec_add(phit, vec_mult1(nhit, bias)), reflDir, sps, depth + 1);
        surfaceColor = color_blend(reflection, surfaceColor, nearestSp->refl);
    }

    // TODO: is this right? compute light contribution everytime? (should make sense)
    float transmission = 1;
    Vec3 lightDir = vec_sub(sps[OBJS_IN_SCENE].c, phit); // last element
    lightDir = vec_normalized(lightDir);
    
    // CHECK FOR SHADOW RAY
    for (int j = 0; j < OBJS_IN_SCENE; j++)
    {
        float t_shadow = sphere_ray_intersection(vec_add(phit, vec_mult1(nhit, bias)), lightDir, sps[j]);
        if (t_shadow > 0)
        {
            transmission = 0;
            break;
        }
    }
    
    // IF NO SHADOW RAY - COMPUTE LIGHT CONTRIBUTION
    if (transmission != 0)
    {
        Vec3 lightColRatio = vec_mult1(nearestSp->surfCol, transmission);
        lightColRatio = vec_mult1(lightColRatio, max(0, vec_dot(nhit, lightDir)));
        surfaceColor = vec_add(surfaceColor, vec_mult(lightColRatio, sps[OBJS_IN_SCENE].emissionColor)); 
    }

    return surfaceColor;
}


void render(Sphere sps[])
{
    unsigned width = 1280, height = 720;
    float invWidth = 1 / (float)width;
    float invHeight = 1 / (float)height;
    float fov = 30;
    float aspectratio = width / (float)height;
    float angle = tan(M_PI * 0.5 * fov / 180.);

    Camera* camera = create_camera(fov, width, height, 0, 1000);

    //translation example
    Vec3 t = {0.0,0.0,0.0};
    move_camera(camera, t);

    FILE *f = fopen("render_c.ppm", "w");
    fprintf(f, "P3 \n%d %d 255\n", width, height);

    // TRACE
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            Vec3 dir = shoot_ray(camera, x, y);

            Vec3 px_col = trace(new_vector(0, 0, 0), dir, sps, 0);
            px_col = vec_mult1(px_col, 1./4);  // TODO: should divide by a counter of successful bounces instead

            fprintf(f, "%d",     (int)(min(1, px_col.x) * 255));
            fprintf(f, " %d",    (int)(min(1, px_col.y) * 255));
            fprintf(f, " %d \n", (int)(min(1, px_col.z) * 255));
        }   
    }

    fclose(f);
}

int main()
{
    Sphere sp0; sp0.c = new_vector(0, -10004, -20); sp0.r = 10000; sp0.r2 = 100000000; sp0.surfCol = new_vector(0.2, 0.3, 0.8); sp0.refl=0; sp0.emissionColor = new_vector(0,0,0);
    Sphere sp1; sp1.c = new_vector(4, 0, -25); sp1.r = 3; sp1.r2 = 9; sp1.surfCol = new_vector(1, 0.32, 0.36); sp1.refl=0; sp1.emissionColor = new_vector(0,0,0);
    Sphere sp2; sp2.c = new_vector(-4, 0, -15); sp2.r = 3; sp2.r2 = 9; sp2.surfCol = new_vector(0.3, 1, 0.36); sp2.refl=0; sp2.emissionColor = new_vector(0,0,0);
    Sphere sp3; sp3.c = new_vector(0, 0, -40); sp3.r = 3; sp3.r2 = 9; sp3.surfCol = new_vector(0.65, 0.77, 0.97); sp3.refl=0.3; sp3.emissionColor = new_vector(0,0,0);
    // Sphere sp4; sp4.c = new_vector(-5.5, 0, -15); sp4.r = 3; sp4.r2 = 9; sp4.surfCol = new_vector(0.9, 0.9, 0.9); sp4.emissionColor = new_vector(0,0,0);

    double em = 4;
    Sphere sp5; sp5.c = new_vector(0, 20, -15); sp5.r = 3; sp5.r2 = 9; sp5.surfCol = new_vector(0, 0, 0); sp5.emissionColor = new_vector(em, em, em);

    Sphere sps[OBJS_IN_SCENE+1];
    sps[0] = sp0;
    sps[1] = sp1;
    sps[2] = sp2;
    sps[3] = sp3;
    sps[4] = sp5;
    // sps[5] = sp5;

    render(sps);
    return 0;
}