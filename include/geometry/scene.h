#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "material.h"
#include "geometry/transform.h"
#include "light.h"
#include "vec3.h"

// typedef float (*sdf_func)(const float *vec_p, float *params);
typedef void (*sdf_func)(const SIMD_VEC_PS* simd_vec_p, float *params, SIMD_MMS* simd_mmd_dists);

typedef struct
{
    sdf_func sdf;
    float *params;
    const Material *mat;
    const Transform *transform;

} GeomtericObject;

typedef struct
{
    char *name;

    unsigned char *img;

    Camera *camera;
    PointLight *light;

    int nr_materials;
    Material **materials;

    int nr_transforms;
    Transform **transforms;

    int nr_geom_objs;
    GeomtericObject **geometric_ojects;
} Scene;

typedef struct
{
    int finish_ray_mask_int; 
    SIMD_MMS finish_ray_mask; // 1 if a ray is done -> don't change other values for this entry
    SIMD_MMS min_dist;
    SIMD_MMS nearest_obj_idx;
    SIMD_MMS intersected_mask;
    SIMD_VEC_PS intersection_pt; 
    SIMD_MMS s;
} SDF_Info;

// void sdf(float vec_p[NR_VEC_ELEMENTS], const Scene *scene, SDF_Info *sdf_info_out);
void sdf(const SIMD_VEC_PS *simd_vec_p, const Scene *scene, SDF_Info *sdf_info_out);

Scene *build_scene(const char *name);

// void apply_transform(float vec_p[NR_VEC_ELEMENTS], const Transform *tr, float vec_res[NR_VEC_ELEMENTS]);
void apply_transform(const SIMD_VEC_PS* simd_vec_p, const Transform *tr, SIMD_VEC_PS* simd_vec_transf_pt);

void create_image(Scene* scene, unsigned int width, unsigned int height);

void destroy_image(Scene* scene);

#endif // SCENE_H_