#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "material.h"
#include "geometry/transform.h"
#include "light.h"
#include "vec3.h"

typedef double (*sdf_func)(const double *vec_p, double *params);

typedef struct
{
    sdf_func sdf;
    double *params;
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
    SIMD_MMD min_dist;
    SIMD_MMD nearest_obj_idx;
    SIMD_MMD intersected;
    double intersection_pt[NR_VEC_ELEMENTS]; // TODO: change this to be a SIMD_VEC
    SIMD_MMD s;
} SDF_Info;

// void sdf(double vec_p[NR_VEC_ELEMENTS], const Scene *scene, SDF_Info *sdf_info_out);
void sdf(SIMD_VEC *simd_vec_p, const Scene *scene, SDF_Info *sdf_info_out);

Scene *build_scene(const char *name);

// void apply_transform(double vec_p[NR_VEC_ELEMENTS], const Transform *tr, double vec_res[NR_VEC_ELEMENTS]);
void apply_transform(SIMD_VEC* simd_vec_p, const Transform *tr, SIMD_VEC* simd_vec_transf_pt);

void create_image(Scene* scene, unsigned int width, unsigned int height);

void destroy_image(Scene* scene);

#endif // SCENE_H_