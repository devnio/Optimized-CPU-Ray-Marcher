#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "geometry/box.h"
#include "geometry/cone.h"
#include "geometry/octahedron.h"
#include "geometry/torus.h"
#include "scene_loader.h"

#define MAX_NR_SCENES 10
int num_scenes = 0;
scene_builder scenes_builder[MAX_NR_SCENES];

/*
  Add geometric object directly to the scene.
  - Specify the sdf function pointer: which defines the shape.
  - The material, transform and the rest of the parameters that the sdf takes.
*/
void add_geom_obj_to_scene(Scene *scene, sdf_func sdf, const Material *mat, const Transform *transform, int nr_params, ...)
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
    const Material *white = new_material(new_vector(1.0, 1.0, 1.0), 0.0, 15.0, new_vector(0.0, 0.0, 0.0));
    const Material *blue = new_material(new_vector(0, 0.1, 0.6), 0.0, 15.0, new_vector(0.0, 0.0, 0.0));
    const Material *green = new_material(new_vector(0, 0.6, 0.1), 0.0, 15.0, new_vector(0.0, 0.0, 0.0));
    const Material *yellow = new_material(new_vector(0.5, 0.5, 0.0), 0.0, 15.0, new_vector(0.0, 0.0, 0.0));
    Material *refl = new_material(new_vector(0.3, 0.0, 0.0), 0.5, 15.0, new_vector(0.0, 0.0, 0.0));
    const Transform *identity = new_transform(new_vector(0.0, 0.0, 0.0), new_vector(0.0, 0.0, 0.0));
    double h = -1.5;
    const Transform *pos_box = new_transform(new_vector(-4.0, h, 15.0), new_vector(0.0, 0.0, 0.0));
    const Transform *pos_octahedron  = new_transform(new_vector(-1.5, h, 12.0), new_vector(0.0, 0.0, 0.0));
    const Transform *pos_cone = new_transform(new_vector(1.5, h, 12.0), new_vector(0.0, 0.0, 0.0));
    const Transform *pos_torus = new_transform(new_vector(4.0, h, 15.0), new_vector(0.0, 0.0, 0.0));
    const Transform *pos_sp = new_transform(new_vector(0.0, h+1.5, 20.0), new_vector(0.0, 0.0, 0.0));

    add_geom_obj_to_scene(scene, &sdf_plane, white, identity, nr_plane_params, 0.0, 1.0, 0.0, 3.0);  
    add_geom_obj_to_scene(scene, &sdf_box, blue, pos_box, nr_box_params, 0.25, 0.5, 1.0);
    add_geom_obj_to_scene(scene, &sdf_sphere, refl, pos_sp, nr_sphere_params, 3.0);  
    add_geom_obj_to_scene(scene, &sdf_cone, blue, pos_cone, nr_cone_params, 1.0, 0.5, 1.0);
    add_geom_obj_to_scene(scene, &sdf_torus, yellow, pos_torus, nr_torus_params, 1.0, 0.2);
    add_geom_obj_to_scene(scene, &sdf_octahedron, green, pos_octahedron, nr_octahedron_params, 1.0);
    
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
    // TODO: make sure you free only once -> create an array in scene with the global transforms and materials and so on.
    // for (int i = 0; i < scene->nr_geom_objs; ++i)
    // {
    //     GeomtericObject *p = (scene->geometric_ojects)[i];
    //     free((Material*)p->mat);
    //     p->mat = NULL;
    //     free((Transform*)p->transform);
    //     p->transform = NULL;
    //     free(p->params);
    //     p->params = NULL;
    //     free(p);
    //     p = NULL;
    // }
    free(scene->geometric_ojects);
    free(scene->name);
    // free(scene);
    // scene = 0;
}
