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
    
    SIMD_MMD simd_vec_scale, simd_vec_aspectRatio, simd_vec_widthPx;
    SIMD_MMD tmp1, tmp2, x__, y__, z__;
    SIMD_MMD const1;
    SIMD_MMD const2;

    const1 = SET_PD(3, 2, 1, 0);

    float x0 = (2 * (i + 0.5) / (float)camera->widthPx - 1) * camera->aspectRatio * camera->scale;
    // float x1 = (2 * ((i + 1) + 0.5) / (float)camera->widthPx - 1) * camera->aspectRatio * camera->scale;
    // float x2 = (2 * ((i + 2) + 0.5) / (float)camera->widthPx - 1) * camera->aspectRatio * camera->scale;
    // float x3 = (2 * ((i + 3) + 0.5) / (float)camera->widthPx - 1) * camera->aspectRatio * camera->scale;

    simd_vec_scale = SET1_PD(camera->scale);
    simd_vec_aspectRatio = SET1_PD(camera->aspectRatio);
    simd_vec_widthPx = SET1_PD(camera->widthPx);

    tmp1 = DIV_PD(MULT_PD(SET1_PD(2), ADD_PD(ADD_PD(SET1_PD(i), const1), SET1_PD(0.5))), simd_vec_widthPx);
    tmp1 = SUB_PD(tmp1, SET1_PD(1));
    // tmp2 = ADD_PD(ADD_PD(SET1_PD(i), const1), SET1_PD(0.5));
    x__ = MULT_PD(tmp1, MULT_PD(simd_vec_scale, simd_vec_aspectRatio));
    // xxxx

    float y = (1 - 2 * (j + 0.5) / (float)camera->heightPx) * camera->scale;
    y__ = SET1_PD((double) y);

    z__ = SET1_PD(camera->dir[2]);

    Mat4 *m = &(camera->viewMatrix);

    SIMD_VEC __simd_vec_dir;
    SIMD_MMD simd_mmd_zero = _mm256_setzero_pd();

    __simd_vec_dir.x = _mm256_fmadd_pd(x__, SET1_PD(m->m[0][0]), simd_mmd_zero); // xxxx
    __simd_vec_dir.y = _mm256_fmadd_pd(x__, SET1_PD(m->m[1][0]), simd_mmd_zero); // yyyy
    __simd_vec_dir.z = _mm256_fmadd_pd(x__, SET1_PD(m->m[2][0]), simd_mmd_zero); // zzzz

    __simd_vec_dir.x = _mm256_fmadd_pd(y__, SET1_PD(m->m[0][1]), __simd_vec_dir.x); // xxxx
    __simd_vec_dir.y = _mm256_fmadd_pd(y__, SET1_PD(m->m[1][1]), __simd_vec_dir.y); // yyyy
    __simd_vec_dir.z = _mm256_fmadd_pd(y__, SET1_PD(m->m[2][1]), __simd_vec_dir.z); // zzzz

    __simd_vec_dir.x = _mm256_fmadd_pd(z__, SET1_PD(m->m[0][2]), __simd_vec_dir.x); // xxxx
    __simd_vec_dir.y = _mm256_fmadd_pd(z__, SET1_PD(m->m[1][2]), __simd_vec_dir.y); // yyyy
    __simd_vec_dir.z = _mm256_fmadd_pd(z__, SET1_PD(m->m[2][2]), __simd_vec_dir.z); // zzzz

    simd_vec_normalize(&__simd_vec_dir, simd_vec_dir);
}


void free_camera(Camera *camera)
{
    free(camera);
}
