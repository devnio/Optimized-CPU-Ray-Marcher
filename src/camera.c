#undef __STRICT_ANSI__ // on win: for M_PI, remove guards in math.h

#include <stdlib.h>
#include <math.h>
#include <stdalign.h>
#include "simd.h"
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

void shoot_rays(Camera *camera, double i, double j, SIMD_VEC *simd_vec_dir)
{
    //Normalize screen coordinates
    float y = (1 - (j + 0.5) * _div_heightPx) * camera->scale;

    SIMD_MMD _const, tmp, x_vec, y_vec, z_vec;

    _const = SET_PD(3, 2, 1, 0);

    tmp = ADD_PD(ADD_PD(SET1_PD(i), _const), SET1_PD(0.5));
    tmp = MULT_PD(tmp, SET1_PD((double) _div_widthPx));
    tmp = SUB_PD(tmp, SET1_PD(1));

    x_vec = MULT_PD(tmp, MULT_PD(SET1_PD(camera->scale), SET1_PD(camera->aspectRatio)));
    y_vec = SET1_PD((double) y);
    z_vec = SET1_PD(camera->dir[2]);

    Mat4 *m = &(camera->viewMatrix);

    simd_vec_dir->x = MULT_PD(x_vec, SET1_PD(m->m[0][0])); 
    simd_vec_dir->y = MULT_PD(x_vec, SET1_PD(m->m[1][0])); 
    simd_vec_dir->z = MULT_PD(x_vec, SET1_PD(m->m[2][0])); 

    simd_vec_dir->x = FMA_PD(y_vec, SET1_PD(m->m[0][1]), simd_vec_dir->x); 
    simd_vec_dir->y = FMA_PD(y_vec, SET1_PD(m->m[1][1]), simd_vec_dir->y); 
    simd_vec_dir->z = FMA_PD(y_vec, SET1_PD(m->m[2][1]), simd_vec_dir->z); 

    simd_vec_dir->x = FMA_PD(z_vec, SET1_PD(m->m[0][2]), simd_vec_dir->x); 
    simd_vec_dir->y = FMA_PD(z_vec, SET1_PD(m->m[1][2]), simd_vec_dir->y); 
    simd_vec_dir->z = FMA_PD(z_vec, SET1_PD(m->m[2][2]), simd_vec_dir->z); 

    simd_vec_normalize(simd_vec_dir, simd_vec_dir);
}


void free_camera(Camera *camera)
{
    free(camera);
}
