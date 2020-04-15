#undef __STRICT_ANSI__ // on win: for M_PI, remove guards in math.h

#include<stdlib.h>
#include "camera.h"
#include <math.h>


Vec3 up = {0.,1.,0.};



struct Camera* create_camera(float fov, unsigned int widthPx, unsigned int heightPx, float near, float far){

    Camera* camera = malloc(sizeof(Camera));
    camera->pos = (Vec3) {0.,0.,0.};
    camera->dir = (Vec3) {0, 0, 1};
    camera->fov = fov;
    camera->widthPx = widthPx;
    camera->heightPx = heightPx;
    camera->near = near;
    camera->far = far;

    camera->aspectRatio = widthPx/(float)heightPx;

    camera->scale = tan(degrees_to_rad(camera->fov * 0.5));
    camera->viewMatrix = look_at(camera->pos, camera->dir, up);
    return camera;
}

float degrees_to_rad(float x){
    return x * M_PI / 180.0;
}

void move_camera(Camera *camera, Vec3 t){
    camera->pos = vec_add(camera->pos, t);
    camera->viewMatrix = look_at(camera->pos, camera->dir, up);
}

void rotate_camera(Camera *camera, double xRot, double yRot){

    xRot = degrees_to_rad(xRot);
    yRot = degrees_to_rad(yRot);

    Vec3 xAxis = new_vector(camera->viewMatrix.m[0][0], camera->viewMatrix.m[1][0], camera->viewMatrix.m[2][0]);
    Vec3 yAxis = new_vector(camera->viewMatrix.m[0][1], camera->viewMatrix.m[1][1], camera->viewMatrix.m[2][1]);

    camera->dir = vec_rotate(camera->dir, xAxis, xRot);
    camera->dir = vec_rotate(camera->dir, yAxis, yRot);
    
    camera->viewMatrix = look_at(camera->pos, camera->dir, up);
}

Vec3 shoot_ray(Camera *camera, int i, int j){

    //Normalize screen coordinates
    float x = (2 * (i + 0.5) / (float)camera->widthPx - 1) * camera->aspectRatio * camera->scale; 
    float y = (1 - 2 * (j + 0.5) / (float)camera->heightPx) * camera->scale; 

    Vec3 dir = new_vector(x, y, camera->dir.z);

    Vec3 sRay = mult_vec_matrix_no_homo(&(camera->viewMatrix), dir);

    return vec_normalized(sRay);
}


void free_camera(Camera *camera)
{
	free(camera);
}


