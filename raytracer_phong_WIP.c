#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

// MACROS
#define MAX_RAY_DEPTH 4
#define OBJS_IN_SCENE 4

/* ===============
 * Vector 3 - START
 * =============== */

typedef struct
{
    double x, y, z;
} Vector3;

Vector3 new_vector(double x, double y, double z)
{
    Vector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

Vector3 vec_mult(Vector3 v1, Vector3 v2)
{
    return new_vector(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

Vector3 vec_mult1(Vector3 v, double m)
{
    return new_vector(v.x * m, v.y * m, v.z * m);
}

Vector3 vec_add(Vector3 v1, Vector3 v2)
{
    return new_vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 vec_add1(Vector3 v, double m)
{
    return new_vector(v.x + m, v.y + m, v.z + m);
}

Vector3 vec_sub(Vector3 v1, Vector3 v2)
{
    return new_vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

double vec_norm(Vector3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 vec_normalized(Vector3 v)
{
    double norm = vec_norm(v);
    return vec_mult1(v, 1 / norm);
}

double vec_dot(Vector3 v1, Vector3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 vec_reflect(Vector3 v, Vector3 normal)
{
    return vec_sub(v, vec_mult1(vec_mult1(normal, vec_dot(v, normal)), 2));
}

/* ===============
 * Vector 3 - END
 * =============== */

/* ===============
 * Sphere - START
 * =============== */
typedef struct
{
    Vector3 c;
    double r;
    double r2;
    Vector3 surfCol;
    double refl;
    Vector3 emissionColor;
    // for now leave out emission, transparency, reflectance
} Sphere;

double sphere_ray_intersection(Vector3 o, Vector3 dir, Sphere s)
{
    Vector3 l = vec_sub(s.c, o);
    float tca = vec_dot(l, dir);
    if (tca < 0)
        return -1;

    float d2 = vec_dot(l, l) - tca * tca;
    if (d2 > s.r2)
        return -1;

    float thc = sqrt(s.r2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 0)
        return t1;
    else
        return t0;
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
    if (a < b)
        return a;
    else
        return b;
}

double max(double a, double b)
{
    if (a < b)
        return b;
    else
        return a;
}

Vector3 color_addWeighted(Vector3 colA, Vector3 colB, double weightA, double weightB)
{
    double sum = weightA + weightB;
    Vector3 c;
    c.x = ((colA.x * weightA) + (colB.x * weightB)) / sum;
    c.y = ((colA.y * weightA) + (colB.y * weightB)) / sum;
    c.z = ((colA.z * weightA) + (colB.z * weightB)) / sum;
    return c;
}

Vector3 color_blend(Vector3 colA, Vector3 colB, double weightA)
{
    double wA = weightA > 1 ? 1 : weightA;
    double weightB = 1 - wA;
    Vector3 c;
    return color_addWeighted(colA, colB, wA, weightB);
}

/* ===============
 * HELPER - END
 * =============== */

Vector3 trace(Vector3 o, Vector3 dir, Sphere sps[], int depth)
{

    // SOME GLOBAL VARIABLES

    float shininess = 15;
    float specular = 0;
    Vector3 ambientColor = new_vector(0, 0, 0); //new_vector(0, 0, 0)
    Vector3 finalColor = new_vector(0, 0, 0);

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
    if (!nearestSp)
        return new_vector(0, 0, 0);
    float bias = 1e-4;
    Vector3 surfaceColor = nearestSp->surfCol;

    // Hit point
    Vector3 phit = vec_add(o, vec_mult1(dir, tnear));

    // Normal
    Vector3 N = vec_sub(phit, nearestSp->c);
    N = vec_normalized(N);

    // TODO: in theory not necessary if normals are computed outwards
    if (vec_dot(dir, N) > 0)
    {
        N = vec_mult1(N, -1);
    }

    // Light dir
    Vector3 L = vec_sub(sps[OBJS_IN_SCENE].c, phit); // last element
    L = vec_normalized(L);

    // printf("Light dir: %f, %f, %f", L.x, L.y, L.z);
    // printf("Normal dir: %f, %f, %f", N.x, N.y, N.z);

    if ((depth < MAX_RAY_DEPTH) && (nearestSp->refl > 0))
    {
        // COMPUTE reflection value
        Vector3 reflDir = vec_sub(dir, vec_mult1(vec_mult1(N, vec_dot(dir, N)), 2));
        reflDir = vec_normalized(reflDir);

        Vector3 reflectedCol = trace(vec_add(phit, vec_mult1(N, bias)), reflDir, sps, depth + 1);
        //surfaceColor =  vec_add(reflectionCol, vec_mult1(surfaceColor, nearestSp->refl));  // color_blend(reflection, surfaceColor, nearestSp->refl);
        finalColor = vec_mult1(reflectedCol, nearestSp->refl);
    }

    // Before doing anything else check if shadow ray
    for (int j = 0; j < OBJS_IN_SCENE; j++)
    {
        float t_shadow = sphere_ray_intersection(vec_add(phit, vec_mult1(N, bias)), L, sps[j]);
        if (t_shadow > 0)
        {
            return vec_add(finalColor, ambientColor);
        }
    }

    // Lamber's cosine law
    float lambertian = max(vec_dot(N, L), 0.0);
    if (lambertian > 0.0)
    {
        // printf("Lambertian: %f", lambertian);
        // Light reflected on normal
        Vector3 R = vec_reflect(vec_mult1(L, -1), N);
        Vector3 V = vec_normalized(vec_mult1(dir, -1));

        // specular term
        float specAngle = max(vec_dot(R, V), 0.0);
        specular = pow(specAngle, shininess);
    }

    Vector3 diffuseColor = vec_mult1(surfaceColor, lambertian);
    // if (nearestSp->refl > 0) diffuseColor = new_vector(0,0,0);
    float specPow = 0.2;
    Vector3 specularColor = new_vector(specPow, specPow, specPow);
    specularColor = vec_mult(sps[OBJS_IN_SCENE].emissionColor, vec_mult1(specularColor, specular));
    finalColor = vec_add(finalColor, vec_add(vec_add(ambientColor, diffuseColor), specularColor));

    // float bias = 1e-4; // add some bias to the point from which we will be tracing
    // if (vec_dot(dir, nhit) > 0)
    // {
    //     nhit = vec_mult1(nhit, -1);
    // }

    // if ((depth < MAX_RAY_DEPTH) && (nearestSp->refl > 0))
    // {
    //     // COMPUTE reflection value
    //     Vector3 reflDir = vec_sub(dir, vec_mult1(vec_mult1(nhit, vec_dot(dir, nhit)), 2));
    //     reflDir = vec_normalized(reflDir);

    //     Vector3 reflection = trace(vec_add(phit, vec_mult1(nhit, bias)), reflDir, sps, depth + 1);
    //     surfaceColor = color_blend(reflection, surfaceColor, nearestSp->refl);
    // }

    // // TODO: is this right? compute light contribution everytime? (should make sense)
    // float transmission = 1;
    // Vector3 lightDir = vec_sub(sps[OBJS_IN_SCENE].c, phit); // last element
    // lightDir = vec_normalized(lightDir);

    // // CHECK FOR SHADOW RAY
    // for (int j = 0; j < OBJS_IN_SCENE; j++)
    // {
    //     float t_shadow = sphere_ray_intersection(vec_add(phit, vec_mult1(nhit, bias)), lightDir, sps[j]);
    //     if (t_shadow > 0)
    //     {
    //         transmission = 0;
    //         break;
    //     }
    // }

    // // IF NO SHADOW RAY - COMPUTE LIGHT CONTRIBUTION
    // if (transmission != 0)
    // {
    //     Vector3 lightColRatio = vec_mult1(nearestSp->surfCol, transmission);
    //     lightColRatio = vec_mult1(lightColRatio, max(0, vec_dot(nhit, lightDir)));
    //     surfaceColor = vec_add(surfaceColor, vec_mult(lightColRatio, sps[OBJS_IN_SCENE].emissionColor));
    // }

    return finalColor;
}

void render(Sphere sps[])
{
    float multiplier = 1.0;

    unsigned width = 1280, height = 720;
    float invWidth = 1 / (float)width;
    float invHeight = 1 / (float)height;
    float fov = 30;
    float aspectratio = width / (float)height;
    float angle = tan(M_PI * 0.5 * fov / 180.);

    FILE *f = fopen("render_c.ppm", "w");
    fprintf(f, "P3 \n%d %d 255\n", width, height);

    // TRACE
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            float xx = 0 + (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = 0 + (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vector3 dir = new_vector(xx, yy, -0.7);
            dir = vec_normalized(dir);

            Vector3 px_col = trace(new_vector(0, 0, 0), dir, sps, 0);
            px_col = vec_mult1(px_col, multiplier); // TODO: should divide by a counter of successful bounces instead

            fprintf(f, "%d", (int)(min(1, px_col.x) * 255));
            fprintf(f, " %d", (int)(min(1, px_col.y) * 255));
            fprintf(f, " %d \n", (int)(min(1, px_col.z) * 255));
        }
    }

    fclose(f);
}

int main()
{
    Sphere sp0;
    sp0.c = new_vector(0, -10004, -20);
    sp0.r = 10000;
    sp0.r2 = 100000000;
    sp0.surfCol = new_vector(0.2, 0.3, 0.8);
    sp0.refl = 0;
    sp0.emissionColor = new_vector(0, 0, 0);
    Sphere sp1;
    sp1.c = new_vector(4, 0, -25);
    sp1.r = 3;
    sp1.r2 = 9;
    sp1.surfCol = new_vector(0.8, 0, 0);
    sp1.refl = 0;
    sp1.emissionColor = new_vector(0, 0, 0);
    Sphere sp2;
    sp2.c = new_vector(-4, 0, -15);
    sp2.r = 3;
    sp2.r2 = 9;
    sp2.surfCol = new_vector(0.3, 1, 0.36);
    sp2.refl = 0;
    sp2.emissionColor = new_vector(0, 0, 0);
    Sphere sp3;
    sp3.c = new_vector(0, 0, -40);
    sp3.r = 3;
    sp3.r2 = 9;
    sp3.surfCol = new_vector(0.2, 0.2, 0.97);
    sp3.refl = 1;
    sp3.emissionColor = new_vector(0, 0, 0);
    // Sphere sp4; sp4.c = new_vector(-5.5, 0, -15); sp4.r = 3; sp4.r2 = 9; sp4.surfCol = new_vector(0.9, 0.9, 0.9); sp4.emissionColor = new_vector(0,0,0);

    double em = 2;
    Sphere sp5;
    sp5.c = new_vector(0, 20, -15);
    sp5.r = 3;
    sp5.r2 = 9;
    sp5.surfCol = new_vector(0, 0, 0);
    sp5.emissionColor = new_vector(em, em, em);

    Sphere sps[OBJS_IN_SCENE + 1];
    sps[0] = sp0;
    sps[1] = sp1;
    sps[2] = sp2;
    sps[3] = sp3;
    sps[4] = sp5;
    // sps[5] = sp5;

    render(sps);
    return 0;
}