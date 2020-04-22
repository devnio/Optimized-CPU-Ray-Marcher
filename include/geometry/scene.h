#ifndef SCENE_H_
#define SCENE_H_

#include "geometry/plane.h"
#include "geometry/octahedron.h"
#include "geometry/sphere.h"
#include "geometry/box.h"
#include "geometry/cone.h"
#include "geometry/torus.h"
#include "light.h"

typedef enum
{
    T_Plane,
    T_Sphere,
    T_Box,
    T_Octahedron,
    T_Cone,
    T_TORUS
} GeometryType; 

typedef struct {
    char* name;

    int nr_planes;
    int nr_octahedrons;
    int nr_spheres;
    int nr_boxes;
    int nr_cones;
    int nr_toruses;
    int nr_lights;

    Plane **planes;
    Octahedron **octahedrons;
    Sphere **spheres; 
    Box **boxes; 
    Cone **cones;
    Torus **toruses;

    PointLight plight;

} Scene;

typedef struct  
{
    double min_dist;
    GeometryType nearest_obj_type;
    int nearest_obj_idx;
    int intersected;
    Vec3 intersection_pt; // if intersected is 0, this shouldn't be used (TODO: maybe separate this?)
    double s;
} SDF_Info;

SDF_Info sdf(Vec3 p, Scene scene, SDF_Info* prev_sdf_info);

Scene* build_scene(int nr_planes, int nr_spheres, int nr_boxes, int nr_cones, int nr_octahedrons, int nr_toruses, PointLight pl, const char* name);

#endif // SCENE_H_