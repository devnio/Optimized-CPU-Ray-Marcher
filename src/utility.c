#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utility.h"

/*
 * Function:  mix 
 * --------------------
 *  Mix two doubles with a weithing 
 *
 *  a: weighted with the third parameter weight
 *  b: weighted with the value (1-weight)
 *  weight: weighting parameter
 *
 *  returns: The weighted value between a and b
 */
double mix(double a, double b, double weight)
{
    return b * weight + a * (1 - weight);
}

/*
 * Function:  min 
 * --------------------
 *  Compute the minimum between a and b 
 *
 *  a: double 
 *  b: double
 *
 *  returns: minimum of a and b
 */
double min(double a, double b)
{
    if (a < b)
        return a;
    else
        return b;
}

/*
 * Function:  max 
 * --------------------
 *  Compute the maximum between a and b 
 *
 *  a: double 
 *  b: double
 *
 *  returns: maximum of a and b
 */
double max(double a, double b)
{
    if (a < b)
        return b;
    else
        return a;
}

/*
 * Function:  clamp 
 * --------------------
 * clamps a value between a minimum and maximum value
 *
 *  value: the value to clamp
 *  min: minimum bound
 *  max: maximum bound
 * 
 *  returns: the input value clamped between bounds
 */
double clamp(double val, double min, double max)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

double sign(double val)
{
    return val / fabs(val);
}

FORCE_INLINE void rotate_point_xyz(const double vec_p[NR_VEC_ELEMENTS], const double* precomp_orient, double vec_res[NR_VEC_ELEMENTS])
{
    double v__q[NR_VEC_ELEMENTS];
    v__q[0] = vec_p[0];
    v__q[1] = vec_p[1];
    v__q[2] = vec_p[2];

    v__q[1] = precomp_orient[0] * vec_p[1] - precomp_orient[1] * vec_p[2];
    v__q[2] = precomp_orient[1] * vec_p[1] + precomp_orient[0] * vec_p[2];

    double v__m[NR_VEC_ELEMENTS];
    v__m[0] = v__q[0];
    v__m[1] = v__q[1];
    v__m[2] = v__q[2];

    v__q[0] = precomp_orient[2] * v__m[0] - precomp_orient[3] * v__m[2];

    vec_res[0] = precomp_orient[4] * v__q[0] - precomp_orient[5] * v__q[1];
    vec_res[1] = precomp_orient[5] * v__q[0] + precomp_orient[4] * v__q[1];
    vec_res[2] = precomp_orient[3] * v__m[0] + precomp_orient[2] * v__m[2];
}


double to_radians(double degrees)
{
    return degrees * 0.01745329251;
}

double mod(double x, double y)
{
    return x - y * floor(x/y);
}

char* _concat(const char *str1, const char *str2) { 
    const size_t len1 = strlen(str1);
    const size_t len2 = strlen(str2);
    char *res = malloc(len1 + len2 + 1); // +1 for the null-terminator

    memcpy(res, str1, len1);
    memcpy(res + len1, str2, len2 + 1); // +1 to copy the null-terminator
    return res; // NOTE: your responsibility to free memory! 
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
Example 1
Decode from disk to raw pixels with a single function call

Memory allocation: image.
*/
unsigned char* decodeOneStep(const char* filename, unsigned *out_width, unsigned *out_height, unsigned* error) {
  unsigned char* image = 0;
  
  *error = lodepng_decode32_file(&image, out_width, out_height, filename);
  //if(*error) printf("error %u: %s\n", *error, lodepng_error_text(*error));
  // ERROR IS REPORTED OUTSIDE FOR CLEAN OUTPUT TERMINAL.

  return image;
}

/*
    Save an image to disk.
    If benchmark_mode is used the image will be saved to path dirName.
    Otherwise the path will be generated automatically by the scene name.
*/
void save_image_to_disk(Scene *scene, char* dirName)
{
    if (dirName != NULL)
    {
        // encode image: filepath is built into name
        encodeOneStep(dirName, scene->img, scene->camera->widthPx, scene->camera->heightPx);
    }
    else
    {
        // Build path-name
        char *path = RENDER_OUT;
        char *tmp = _concat(path, scene->name);
        char *filename = _concat(tmp, ".png");

        // encode image
        encodeOneStep(filename, scene->img, scene->camera->widthPx, scene->camera->heightPx);
        printf("\nImage rendered and saved in path folder %s \n", filename);

        // Clean-up allocated strings
        free(tmp);
        free(filename);
    }
}