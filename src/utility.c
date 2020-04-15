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
 * Function:  color_add_weighted 
 * --------------------
 * computes a new color value by mixing two colors each with a weight
 *
 *  colA: first color value as a vector
 *  colB: second color value as a vector
 *  weightA: weight of color A value as a vector
 *  weightb: weight of color B value as a vector
 * 
 *  returns: color vector value by mixing colA with weightA and colB with weightB
 */
Vec3 color_add_weighted(Vec3 colA, Vec3 colB, double weightA, double weightB)
{
    double sum = weightA + weightB;
    Vec3 c;
    c.x = ((colA.x * weightA) + (colB.x * weightB)) / sum;
    c.y = ((colA.y * weightA) + (colB.y * weightB)) / sum;
    c.z = ((colA.z * weightA) + (colB.z * weightB)) / sum;
    return c;
}

/*
 * Function:  color_blend 
 * --------------------
 * computes a new color value by mixing two colors each with a weight
 *
 *  colA: first color value as a vector
 *  colB: second color value as a vector
 *  weight: weight with which to mix the two colors
 * 
 *  returns: color vector value by mixing the two colors with given weight
 */
Vec3 color_blend(Vec3 colA, Vec3 colB, double weight)
{
    double val = weight > 1 ? 1 : weight;
    double weight_ = 1 - val;
    Vec3 c;
    return color_add_weighted(colA, colB, weight, weight_);
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
double clamp(double value, double min, double max){
    double res = value;
    if (value > max)
        res = max;
    if (value < min)
        res = min;
    return res;
}