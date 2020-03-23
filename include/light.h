#ifndef LIGHT
#define LIGHT

#ifndef VEC_3
 #include "vec3.h"
#endif

typedef struct
{
    Vec3 c;
    Vec3 emissionColor;
} PointLight;


#endif