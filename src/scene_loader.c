#include <stdlib.h>
#include <stdarg.h>

#include "geometry/scene.h"
#include "scene_loader.h"

#define MAX_NR_SCENES 10
int num_scenes = 0;
scene_builder scenes_builder[MAX_NR_SCENES];

Scene* scene_switch(){
    Scene* scene = build_scene(1, "../output/scene_switch.png");

    GeomtericObject* pl0 = (GeomtericObject*)malloc(sizeof(GeomtericObject));
    pl0->mat = new_material(new_vector(0, 0.3, 0.6), 0, 15, new_vector(0, 0, 0));
    pl0->transform = new_transform(new_vector(2, 0, 0), new_vector(0, 0, 0));
    double params[4];// = {0.0, 1.0, 0.0, 3.0};
    params[0] = 0.0;
    params[1] = 1.0;
    params[2] = 0.0;
    params[3] = 4.0;
    pl0->params = params;
    pl0->sdf = &sdf_plane;
    scene->geometric_ojects[0] = pl0;

    return scene;
}



/*
*   TODO: here you can add the scenes that have to be added
*   Note: If needed modify MAX_NR_SCENES
*/
void add_scenes(){
    add_scene(&scene_switch);
}

////////////////////////////////  END SCENES DEFINITION /////////////////////////////////////

void add_scene(scene_builder builder){
    scenes_builder[num_scenes] = builder;
    num_scenes++;
}

SceneContainer build_scenes(){
    add_scenes();
    Scene** scenes = (Scene**) malloc(sizeof(Scene*)*num_scenes);
    
    for(int i=0; i<num_scenes; ++i){
        scenes[i] = scenes_builder[i](); // calls the registered scene functions
    }
    SceneContainer scene_container;
    scene_container.num_scenes=num_scenes;
    scene_container.scenes=scenes;

    return scene_container;
}

void destroy_scene(Scene* scene){
    for(int i=0; i<scene->nr_geom_objs;++i){
        GeomtericObject* p = (scene->geometric_ojects)[i];
        free(p);
    }
    free(scene->geometric_ojects);

    free(scene);
    scene=0;

}

