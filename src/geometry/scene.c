#include "geometry/scene.h"
#include <string.h>

SDF_Info sdf(Vec3 p, Scene scene, SDF_Info* prev_sdf_info)
{
    SDF_Info sdf_info;
    sdf_info.min_dist = INFINITY;

    // Check planes
    for (int k = 0; k < scene.nr_planes; k++)
    {
        // check if has to skip (because it's bouncing off this object)
        if (prev_sdf_info != NULL && prev_sdf_info->nearest_obj_type == T_Plane &&  prev_sdf_info->nearest_obj_idx == k)
            continue;

        double dist = sdf_plane(p, (*(scene.planes)[k]));
        if (dist < sdf_info.min_dist)
        {
            sdf_info.min_dist = dist;
            sdf_info.nearest_obj_type = T_Plane;
            sdf_info.nearest_obj_idx = k;
        }
    }

    // Check ocathedrons
    for (int k = 0; k < scene.nr_octahedrons; k++)
    {
        // check if has to skip (because it's bouncing off this object)
        if (prev_sdf_info != NULL && prev_sdf_info->nearest_obj_type == T_Octahedron &&  prev_sdf_info->nearest_obj_idx == k)
            continue;

        double dist = sdf_octahedron(p, (*(scene.octahedrons)[k]));
        if (dist < sdf_info.min_dist)
        {
            sdf_info.min_dist = dist;
            sdf_info.nearest_obj_type = T_Octahedron;
            sdf_info.nearest_obj_idx = k;
        }
    }

    // Check spheres
    for (int k = 0; k < scene.nr_spheres; k++)
    {
        // check if has to skip (because it's bouncing off this object)
        if (prev_sdf_info != NULL && prev_sdf_info->nearest_obj_type == T_Sphere &&  prev_sdf_info->nearest_obj_idx == k)
            continue;

        double dist = sdf_sphere(p, (*(scene.spheres)[k]));
        if (dist < sdf_info.min_dist)
        {
            sdf_info.min_dist = dist;
            sdf_info.nearest_obj_type = T_Sphere;
            sdf_info.nearest_obj_idx = k;
        }
    }

    // Check boxes
    for (int k = 0; k < scene.nr_boxes; k++)
    {
        // check if has to skip (because it's bouncing off this object)
        if (prev_sdf_info != NULL && prev_sdf_info->nearest_obj_type == T_Box &&  prev_sdf_info->nearest_obj_idx == k)
            continue;

        double dist = sdf_box(p, (*(scene.boxes)[k]));
        if (dist < sdf_info.min_dist)
        {
            sdf_info.min_dist = dist;
            sdf_info.nearest_obj_type = T_Box;
            sdf_info.nearest_obj_idx = k;
        }
    }

   
   // check cones 
   for (int k = 0; k < scene.nr_cones; k++)
    {
        // check if has to skip (because it's bouncing off this object)
        if (prev_sdf_info != NULL && prev_sdf_info->nearest_obj_type == T_Cone &&  prev_sdf_info->nearest_obj_idx == k)
            continue;
        
        double dist = sdf_cone(p, (*(scene.cones)[k]));
        //printf("\n CONE %f\n", dist);//TODO
        if (dist < sdf_info.min_dist)
        {   
            sdf_info.min_dist = dist;
            sdf_info.nearest_obj_type = T_Cone;
            sdf_info.nearest_obj_idx = k;
        }
    }

   // check toruses 
   for (int k = 0; k < scene.nr_toruses; k++)
    {
        // check if has to skip (because it's bouncing off this object)
        if (prev_sdf_info != NULL && prev_sdf_info->nearest_obj_type == T_TORUS &&  prev_sdf_info->nearest_obj_idx == k)
            continue;
        
        double dist = sdf_torus(p, (*(scene.toruses)[k]));
        //printf("\n CONE %f\n", dist);//TODO
        if (dist < sdf_info.min_dist)
        {   
            sdf_info.min_dist = dist;
            sdf_info.nearest_obj_type = T_TORUS;
            sdf_info.nearest_obj_idx = k;
        }
    }

    return sdf_info;
}

Scene* build_scene(int nr_planes, int nr_spheres, int nr_boxes, int nr_cones, int nr_octahedrons, int nr_toruses, const char* name){

    Scene* scene = (Scene*) malloc(sizeof(Scene));

    scene->name = strdup(name);
    scene->nr_planes = nr_planes;
    scene->nr_spheres = nr_spheres;
    scene->nr_boxes = nr_boxes;
    scene->nr_cones = nr_cones;
    scene->nr_octahedrons = nr_octahedrons;
    scene->nr_toruses = nr_toruses;

    Plane** planes = (Plane**) malloc(sizeof(Plane*)*scene->nr_planes);
    Sphere** sps =(Sphere**) malloc(sizeof(Sphere*)*scene->nr_spheres);
    Box** boxes = (Box**) malloc(sizeof(Box*)*scene->nr_boxes);
    Cone** cones = (Cone**) malloc(sizeof(Cone*)*scene->nr_cones);
    Octahedron** octahedrons = (Octahedron**) malloc(sizeof(Octahedron*)*scene->nr_octahedrons);
    Torus** toruses = (Torus**) malloc(sizeof(Torus*)*scene->nr_toruses);

    scene->planes = planes;
    scene->spheres = sps;
    scene->boxes = boxes;
    scene->cones = cones;
    scene->octahedrons = octahedrons;
    scene->toruses = toruses;

    return scene;

}