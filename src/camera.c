#include<stdlib.h>
#define _USE_MATH_DEFINES
#include "camera.h"


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
    camera->viewMatrix.m[0][3] += t.x;
    camera->viewMatrix.m[1][3] += t.y;
    camera->viewMatrix.m[2][3] += t.z;
}

//TODO: Need matrix inversion
void rotate_camera(Camera *camera, double xRot, double yRot){
}

Vec3 shoot_ray(Camera *camera, int i, int j){

    //Normalize screen coordinates
    float x = (2 * (i + 0.5) / (float)camera->widthPx - 1) * camera->aspectRatio * camera->scale; 
    float y = (1 - 2 * (j + 0.5) / (float)camera->heightPx) * camera->scale; 

    Vec3 dir = new_vector(x, y, camera->dir.z);

    Vec3 sRay = mult_vec_matrix(&(camera->viewMatrix), dir);

    return vec_normalized(sRay);
}


void free_camera(Camera *camera)
{
	free(camera);
}


