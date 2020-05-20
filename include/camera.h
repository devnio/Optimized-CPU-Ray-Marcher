#ifndef CAMERA
#define CAMERA

#include "mat4.h"
#include "vec3.h"

// extern float _div_widthPx;
// extern float _div_heightPx;

typedef struct Camera
{
    double pos[NR_VEC_ELEMENTS];    // position of camera
    double dir[NR_VEC_ELEMENTS];    // Normalized direction of view of camera
    double fov;                     //field of view of camera
    unsigned int widthPx;           //Width in pixels of screen
    unsigned int heightPx;          //Height in pixels of screen
    double aspectRatio;
    double scale;
    Mat4 viewMatrix;

    double _div_widthPx;
    double _div_heightPx;
} Camera;

struct Camera *create_camera(double fov, unsigned int widthPx, unsigned int heightPx);

void move_camera(Camera *camera, double vec_t[NR_VEC_ELEMENTS]);

void rotate_camera(Camera *camera, double xRot, double yRot);

void shoot_ray(Camera *camera, double i, double j, double vec_sRay_res[NR_VEC_ELEMENTS]);
void shoot_rays(Camera *camera, double i, double j, SIMD_VEC *simd_vec_dir);

void free_camera(Camera *camera);

void update_width_height(Camera* camera, unsigned int width, unsigned int height);

#endif