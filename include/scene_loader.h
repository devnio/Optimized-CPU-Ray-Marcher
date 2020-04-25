#ifndef SCENE_LOADER_H_
#define SCENE_LOADER_H_

#include "geometry/scene.h"

typedef Scene*(*scene_builder)();

typedef struct {
    int num_scenes;
    Scene **scenes;
} SceneContainer;

Scene* create_scene(char* scene_name);
void add_scene(scene_builder builder);
void add_scenes();
SceneContainer build_scenes();
Scene* scene_baseline();
void destroy_scene(Scene* scene);

#endif // SCENE_LOADER_H_