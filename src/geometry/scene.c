#include <stdlib.h>
#include "geometry/scene.h"

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

        double dist = sdf_plane(p, scene.planes[k]);
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

        double dist = sdf_octahedron(p, scene.octahedrons[k]);
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

        double dist = sdf_sphere(p, scene.spheres[k]);
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

        double dist = sdf_box(p, scene.boxes[k]);
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
        
        double dist = sdf_cone(p, scene.cones[k]);
        //printf("\n CONE %f\n", dist);//TODO
        if (dist < sdf_info.min_dist)
        {   
            sdf_info.min_dist = dist;
            sdf_info.nearest_obj_type = T_Cone;
            sdf_info.nearest_obj_idx = k;
        }
    }

    return sdf_info;
}