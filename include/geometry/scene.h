#ifndef SCENE_H_
#define SCENE_H_

#include "geometry/plane.h"
#include "geometry/octahedron.h"
#include "geometry/sphere.h"

typedef enum
{
    T_Plane,
    T_OCTAHEDRON,
    T_Sphere
} GeometryType; 

typedef struct {
    int nr_planes;
    int nr_octahedrons;
    int nr_spheres;
    Plane *planes;
    Octahedron *octahedrons;
    Sphere *spheres; 
} Scene;

typedef struct  
{
    double min_dist;
    GeometryType nearest_obj_type;
    int nearest_obj_idx;
    int intersected;
    Vec3 intersection_pt; // if intersected is 0, this shouldn't be used (TODO: maybe separate this?)
} SDF_Info;

SDF_Info sdf(Vec3 p, Scene scene, SDF_Info* prev_sdf_info);

#endif // SCENE_H_