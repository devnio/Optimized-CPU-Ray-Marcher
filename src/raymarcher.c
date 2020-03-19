#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "lodepng.h"

// MACROS
#define MAX_RAY_DEPTH 4
#define OBJS_IN_SCENE 5
#define SPECULAR_COEFF 0.2

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
typedef struct
{
    Vector3 c;
    Vector3 emissionColor;
} PointLight;

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
    float shininess;
    Vector3 emissionColor;
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
}

double sdf(Vector3 p, Sphere s)
{
    return vec_norm(vec_sub(s.c, p)) - s.r;
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
Vector3 trace(Vector3 o, Vector3 dir, Sphere sps[], PointLight pLight, int depth, int excludeSp)
{
    // SOME GLOBAL VARIABLES
    Vector3 ambientColor = new_vector(0, 0, 0);
    Vector3 finalColor = new_vector(0, 0, 0);
    float specular = 0;

    // CHECK INTERSECTION WITH SCENE
    double minDist = INFINITY;
    Vector3 phit = o;
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
        phit = vec_add(phit, vec_mult1(dir, minDist));
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
    Vector3 surfaceColor = nearestSp->surfCol;

    // Hit point
    // Vector3 phit = vec_add(o, vec_mult1(dir, tnear));
    // Normal
    Vector3 N = vec_sub(phit, nearestSp->c);
    N = vec_normalized(N);

    // In theory not necessary if normals are computed outwards
    if (vec_dot(dir, N) > 0)
    {
        N = vec_mult1(N, -1);
    }

    // Light dir
    Vector3 L = vec_sub(pLight.c, phit);
    L = vec_normalized(L);

    if ((depth < MAX_RAY_DEPTH) && (nearestSp->refl > 0))
    {
        // Compute reflected dir
        Vector3 reflDir = vec_sub(dir, vec_mult1(vec_mult1(N, vec_dot(dir, N)), 2));
        reflDir = vec_normalized(reflDir);

        // Compute reflected color
        Vector3 reflectedCol = trace(vec_add(phit, vec_mult1(N, bias)), reflDir, sps, pLight, depth + 1, currIdx);
        finalColor = vec_mult1(reflectedCol, nearestSp->refl);
    }

    // Before doing anything else check if shadow ray
    for (int j = 0; j < OBJS_IN_SCENE; j++)
    {
        float t_shadow = sphere_ray_intersection(vec_add(phit, vec_mult1(N, bias)), L, sps[j]);
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
        Vector3 R = vec_reflect(vec_mult1(L, -1), N);
        Vector3 V = vec_normalized(vec_mult1(dir, -1));

        // Specular term
        float specAngle = max(vec_dot(R, V), 0.0);
        specular = pow(specAngle, nearestSp->shininess);
    }

    Vector3 diffuseColor = vec_mult1(surfaceColor, lambertian);
    Vector3 specularColor = new_vector(SPECULAR_COEFF, SPECULAR_COEFF, SPECULAR_COEFF);
    specularColor = vec_mult(pLight.emissionColor, vec_mult1(specularColor, specular));
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

void render(Sphere sps[], PointLight pLight)
{
    unsigned int width = 1280;
    unsigned int height = 720;
    float invWidth = 1 / (float)width;
    float invHeight = 1 / (float)height;
    float fov = 30;
    float aspectratio = width / (float)height;
    float angle = tan(M_PI * 0.5 * fov / 180.);

    // TRACE
    size_t png_img_size = width * height * 4 * sizeof(unsigned char);
    unsigned char *img = (unsigned char *)malloc(png_img_size);
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            float xx = 0 + (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float yy = 0 + (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vector3 dir = new_vector(xx, yy, -0.7);
            dir = vec_normalized(dir);

            Vector3 px_col = trace(new_vector(0, 0, 0), dir, sps, pLight, 0, -1);
            px_col = px_col;

            img[y * width * 4 + x * 4 + 0] = (unsigned char)(min(1, px_col.x) * 255);
            img[y * width * 4 + x * 4 + 1] = (unsigned char)(min(1, px_col.y) * 255);
            img[y * width * 4 + x * 4 + 2] = (unsigned char)(min(1, px_col.z) * 255);
            img[y * width * 4 + x * 4 + 3] = 255;
        }
    }

    encodeOneStep("../output/output_img.png", img, width, height);
    free(img);
}

int main()
{
    Sphere sp0;
    sp0.c = new_vector(0, -10004, -20);
    sp0.r = 10000;
    sp0.r2 = 100000000;
    sp0.surfCol = new_vector(0.2, 0.3, 0.8);
    sp0.refl = 0;
    sp0.shininess = 15;
    sp0.emissionColor = new_vector(0, 0, 0);
    Sphere sp1;
    sp1.c = new_vector(4, 0, -25);
    sp1.r = 3;
    sp1.r2 = 9;
    sp1.surfCol = new_vector(0.8, 0, 0);
    sp1.refl = 0.5;
    sp1.shininess = 15;
    sp1.emissionColor = new_vector(0, 0, 0);
    Sphere sp2;
    sp2.c = new_vector(-4, 0, -15);
    sp2.r = 3;
    sp2.r2 = 9;
    sp2.surfCol = new_vector(0.3, 1, 0.36);
    sp2.refl = 0;
    sp2.shininess = 15;
    sp2.emissionColor = new_vector(0, 0, 0);
    Sphere sp3;
    sp3.c = new_vector(0, 0, -40);
    sp3.r = 3;
    sp3.r2 = 9;
    sp3.surfCol = new_vector(0.2, 0.2, 0.97);
    sp3.refl = 0.1;
    sp3.shininess = 15;
    sp3.emissionColor = new_vector(0, 0, 0);
    Sphere sp4;
    sp4.c = new_vector(50, 0, -100);
    sp4.r = 50;
    sp4.r2 = 2500;
    sp4.surfCol = new_vector(0.6, 0.6, 0);
    sp4.refl = 0;
    sp4.shininess = 15;
    sp4.emissionColor = new_vector(0, 0, 0);

    // Lights (in future can be an array)
    PointLight pLight;
    pLight.c = new_vector(0, 550, -15);
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
