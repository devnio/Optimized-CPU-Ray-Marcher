#undef __STRICT_ANSI__ // on win: for M_PI, remove guards in math.h

#include <stdlib.h>
#include <math.h>

#include "camera.h"
#include "utility.h"

double up[NR_VEC_ELEMENTS] = {0.0, 1.0, 0.0};
// double pos[NR_VEC_ELEMENTS] = {0.0, 0.0, 0.0};
// double dir[NR_VEC_ELEMENTS] = {0.0, 0.0, 1.0};

struct Camera *create_camera(double fov, unsigned int widthPx, unsigned int heightPx)
{

    Camera *camera = malloc(sizeof(Camera));
    camera->pos[0] = 0.0;
    camera->pos[1] = 0.0;
    camera->pos[2] = 0.0;
    camera->dir[0] = 0.0;
    camera->dir[1] = 0.0;
    camera->dir[2] = 1.0;
    camera->fov = fov;
    camera->widthPx = widthPx;
    camera->heightPx = heightPx;

    camera->aspectRatio = widthPx / (double)heightPx;

    camera->scale = tan(to_radians(camera->fov * 0.5));
    camera->viewMatrix = look_at(camera->pos, camera->dir, up);
    return camera;
}

void move_camera(Camera *camera, double vec_t[NR_VEC_ELEMENTS])
{
    vec_add(camera->pos, vec_t, camera->pos);
    camera->viewMatrix = look_at(camera->pos, camera->dir, up);
}

void rotate_camera(Camera *camera, double xRot, double yRot)
{

    xRot = to_radians(xRot);
    yRot = to_radians(yRot);

    double v__xAxis[NR_VEC_ELEMENTS];
    v__xAxis[0] = camera->viewMatrix.m[0][0];
    v__xAxis[1] = camera->viewMatrix.m[1][0];
    v__xAxis[2] = camera->viewMatrix.m[2][0];

    double v__yAxis[NR_VEC_ELEMENTS];
    v__yAxis[0] = camera->viewMatrix.m[0][1];
    v__yAxis[1] = camera->viewMatrix.m[1][1];
    v__yAxis[2] = camera->viewMatrix.m[2][1];

    vec_rotate(camera->dir, v__xAxis, xRot, camera->dir);
    vec_rotate(camera->dir, v__yAxis, yRot, camera->dir);

    camera->viewMatrix = look_at(camera->pos, camera->dir, up);
}

void update_width_height(Camera* camera, unsigned int width, unsigned int height) {
    camera->widthPx = width;
    camera->heightPx = height;
}

void shoot_ray(Camera *camera, double i, double j, double vec_sRay_res[NR_VEC_ELEMENTS])
{

    //Normalize screen coordinates
    float x = (2 * (i + 0.5) / (float)camera->widthPx - 1) * camera->aspectRatio * camera->scale;
    float y = (1 - 2 * (j + 0.5) / (float)camera->heightPx) * camera->scale;

    double v__dir[NR_VEC_ELEMENTS];
    v__dir[0] = x;
    v__dir[1] = y;
    v__dir[2] = camera->dir[2];


    mult_vec_matrix_no_homo(&(camera->viewMatrix), v__dir, vec_sRay_res);
    vec_normalize(vec_sRay_res);
}

void free_camera(Camera *camera)
{
    free(camera);
}
