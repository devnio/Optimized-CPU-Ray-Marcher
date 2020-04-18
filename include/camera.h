#ifndef CAMERA
#define CAMERA

#include "mat4.h"


typedef struct Camera {
    Vec3 pos;               //position of camera
    Vec3 dir;               // Normalized direction of view of camera
    double fov;              //field of view of camera
    unsigned int widthPx;   //Width in pixels of screen 
    unsigned int heightPx;  //Height in pixels of screen 
    double aspectRatio;
    double scale;
    Mat4 viewMatrix;
} Camera;


struct Camera* create_camera(double fov, unsigned int widthPx, unsigned int heightPx);

void move_camera(Camera *camera, Vec3 t);

void rotate_camera(Camera *camera, double xRot, double yRot);

Vec3 shoot_ray(Camera *camera, double coordX, double coordY);

void free_camera(Camera *camera);


#endif