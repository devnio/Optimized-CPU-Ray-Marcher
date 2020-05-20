#undef __STRICT_ANSI__ // on win: for M_PI, remove guards in math.h

#include <stdlib.h>
#include <math.h>
#include <stdalign.h>
#include "simd.h"
#include "camera.h"
#include "utility.h"

float up[NR_VEC_ELEMENTS] = {0.0, 1.0, 0.0};
// float pos[NR_VEC_ELEMENTS] = {0.0, 0.0, 0.0};
// float dir[NR_VEC_ELEMENTS] = {0.0, 0.0, 1.0};

struct Camera *create_camera(float fov, unsigned int widthPx, unsigned int heightPx)
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

    camera->aspectRatio = widthPx / (float)heightPx;

    camera->scale = tan(to_radians(camera->fov * 0.5));
    camera->viewMatrix = look_at(camera->pos, camera->dir, up);
    return camera;
}

void move_camera(Camera *camera, float vec_t[NR_VEC_ELEMENTS])
{
    vec_add(camera->pos, vec_t, camera->pos);
    camera->viewMatrix = look_at(camera->pos, camera->dir, up);
}

void rotate_camera(Camera *camera, float xRot, float yRot)
{

    xRot = to_radians(xRot);
    yRot = to_radians(yRot);

    float v__xAxis[NR_VEC_ELEMENTS];
    v__xAxis[0] = camera->viewMatrix.m[0][0];
    v__xAxis[1] = camera->viewMatrix.m[1][0];
    v__xAxis[2] = camera->viewMatrix.m[2][0];

    float v__yAxis[NR_VEC_ELEMENTS];
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
    _div_widthPx = 2 / (float) width;
    _div_heightPx = 2 / (float) height;
}

void shoot_rays(Camera *camera, float i, float j, SIMD_VEC_PS *simd_vec_dir)
{
    //Normalize screen coordinates
    float y = (1 - (j + 0.5) * _div_heightPx) * camera->scale;

    SIMD_MMS _const, tmp, x_vec, y_vec, z_vec;

    _const = SET_PS(7, 6, 5, 4, 3, 2, 1, 0);

    tmp = ADD_PS(ADD_PS(SET1_PS(i), _const), SET1_PS(0.5));
    tmp = MULT_PS(tmp, SET1_PS(_div_widthPx));
    tmp = SUB_PS(tmp, SET1_PS(1));

    x_vec = MULT_PS(tmp, MULT_PS(SET1_PS(camera->scale), SET1_PS(camera->aspectRatio)));
    y_vec = SET1_PS(y);
    z_vec = SET1_PS(camera->dir[2]);

    Mat4 *m = &(camera->viewMatrix);

    simd_vec_dir->x = MULT_PS(x_vec, SET1_PS(m->m[0][0])); 
    simd_vec_dir->y = MULT_PS(x_vec, SET1_PS(m->m[1][0])); 
    simd_vec_dir->z = MULT_PS(x_vec, SET1_PS(m->m[2][0])); 

    simd_vec_dir->x = FMADD_PS(y_vec, SET1_PS(m->m[0][1]), simd_vec_dir->x); 
    simd_vec_dir->y = FMADD_PS(y_vec, SET1_PS(m->m[1][1]), simd_vec_dir->y); 
    simd_vec_dir->z = FMADD_PS(y_vec, SET1_PS(m->m[2][1]), simd_vec_dir->z); 

    simd_vec_dir->x = FMADD_PS(z_vec, SET1_PS(m->m[0][2]), simd_vec_dir->x); 
    simd_vec_dir->y = FMADD_PS(z_vec, SET1_PS(m->m[1][2]), simd_vec_dir->y); 
    simd_vec_dir->z = FMADD_PS(z_vec, SET1_PS(m->m[2][2]), simd_vec_dir->z); 

    /// TODO: inline function
    simd_vec_normalize(simd_vec_dir, simd_vec_dir);
}


void free_camera(Camera *camera)
{
    free(camera);
}
