#ifndef CAMERA
#define CAMERA

#include "mat4.h"
#include "vec3.h"

extern float _div_widthPx;
extern float _div_heightPx;

typedef struct Camera
{
    float pos[NR_VEC_ELEMENTS];    // position of camera
    float dir[NR_VEC_ELEMENTS];    // Normalized direction of view of camera
    float fov;                     //field of view of camera
    unsigned int widthPx;           //Width in pixels of screen
    unsigned int heightPx;          //Height in pixels of screen
    float aspectRatio;
    float scale;
    Mat4 viewMatrix;
} Camera;

struct Camera *create_camera(float fov, unsigned int widthPx, unsigned int heightPx);
void move_camera(Camera *camera, float vec_t[NR_VEC_ELEMENTS]);
void rotate_camera(Camera *camera, float xRot, float yRot);
void shoot_rays(Camera *camera, float i, float j, SIMD_VEC_PS *simd_vec_dir);
void free_camera(Camera *camera);
void update_width_height(Camera* camera, unsigned int width, unsigned int height);

#endif