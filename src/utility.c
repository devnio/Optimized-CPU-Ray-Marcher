#include "utility.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

Vec3 rotate_point_x(Vec3 p, double angle)
{
    double a;
    Vec3 q = p;

    a = to_radians(angle);
    double c = cos(a);
    double s = sin(a);
    q.y = c * p.y - s * p.z;
    q.z = s * p.y + c * p.z;

    return q;
}

Vec3 rotate_point_y(Vec3 p, double angle)
{
    double a;
    Vec3 q = p;
  
    a = to_radians(angle);
    double c = cos(a);
    double s = sin(a);
    q.x = c * p.x - s * p.z;
    q.z = s * p.x + c * p.z;

    return q;
}

Vec3 rotate_point_z(Vec3 p, double angle)
{
    double a;
    Vec3 q = p;
  
    a = to_radians(angle);
    double c = cos(a);
    double s = sin(a);
    q.x = c * p.x - s * p.y;
    q.y = s * p.x + c * p.y;

    return q;
}

double to_radians(double degrees)
{
    return degrees * (M_PI / 180.0);
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