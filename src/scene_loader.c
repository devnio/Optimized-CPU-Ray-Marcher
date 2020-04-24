#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "geometry/plane.h"
#include "geometry/sphere.h"
// #include "geometry/octahedron.h"
// #include "geometry/box.h"
// #include "geometry/cone.h"
// #include "geometry/torus.h"
#include "scene_loader.h"

#define MAX_NR_SCENES 10
int num_scenes = 0;
scene_builder scenes_builder[MAX_NR_SCENES];

/*
  Add geometric object directly to the scene.
  - Specify the sdf function pointer: which defines the shape.
  - The material, transform and the rest of the parameters that the sdf takes.
*/
void add_geom_obj_to_scene(Scene *scene, sdf_func sdf, Material *mat, Transform *transform, int nr_params, ...)
{
    // Get params
    double *params = (double *)malloc(sizeof(double) * nr_params);
    va_list args;
    va_start(args, nr_params);
    for (int i = 0; i < nr_params; i++)
    {
        params[i] = va_arg(args, double);
    }
    va_end(args);

    // Create geometric object
    GeomtericObject *geom_obj = (GeomtericObject *)malloc(sizeof(GeomtericObject));   
    
    // Assign sdf function
    geom_obj->sdf = sdf;

    // Assign material
    geom_obj->mat = mat;
    
    // Assign transform
    geom_obj->transform = transform;

    // Assign params
    geom_obj->params = params;

    // Add geometric object to scene & increment obj counter
    scene->geometric_ojects[scene->nr_geom_objs] = geom_obj;
    scene->nr_geom_objs++;

}

Scene *scene_switch()
{
    Scene *scene = build_scene("../output/scene_switch.png");
    Material *blue = new_material(new_vector(0, 0.1, 0.6), 0.0, 15.0, new_vector(0.0, 0.0, 0.0));
    // Material *blue_refl = new_material(new_vector(0, 0.3, 0.6), 0.5, 15.0, new_vector(0.0, 0.0, 0.0));
    Transform *identity = new_transform(new_vector(0.0, 0.0, 0.0), new_vector(0.0, 0.0, 0.0));
    Transform *pos0 = new_transform(new_vector(0.0, 0.0, 15.0), new_vector(0.0, 0.0, 0.0));

    add_geom_obj_to_scene(scene, &sdf_plane, blue, identity, nr_plane_params, 0.0, 1.0, 0.0, 3.0);
    add_geom_obj_to_scene(scene, &sdf_sphere, blue, pos0, nr_sphere_params, 2.0);
    
    return scene;
}

/*
*   TODO: here you can add the scenes that have to be added
*   Note: If needed modify MAX_NR_SCENES
*/
void add_scenes()
{
    add_scene(&scene_switch);
}

////////////////////////////////  END SCENES DEFINITION /////////////////////////////////////

void add_scene(scene_builder builder)
{
    scenes_builder[num_scenes] = builder;
    num_scenes++;
}

SceneContainer build_scenes()
{
    add_scenes();
    Scene **scenes = (Scene **)malloc(sizeof(Scene *) * num_scenes);

    for (int i = 0; i < num_scenes; ++i)
    {
        scenes[i] = scenes_builder[i](); // calls the registered scene functions
    }
    SceneContainer scene_container;
    scene_container.num_scenes = num_scenes;
    scene_container.scenes = scenes;

    return scene_container;
}

void destroy_scene(Scene *scene)
{
    for (int i = 0; i < scene->nr_geom_objs; ++i)
    {
        GeomtericObject *p = (scene->geometric_ojects)[i];
        free(p->mat);
        free(p->transform);
        free(p->params);
        free(p);
    }
    free(scene->geometric_ojects);
    free(scene);
    scene = 0;
}

////////////////////////////////  JSON DEFINITION /////////////////////////////////////
/*

if (strcmp(obj_name, "Plane"))
{

}

*/
