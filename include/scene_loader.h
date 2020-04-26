#ifndef SCENE_LOADER_H_
#define SCENE_LOADER_H_

#include "geometry/scene.h"

typedef struct {
    int num_scenes;
    Scene **scenes;
} SceneContainer;

Scene *create_scene_from_json(char *scene_name);
SceneContainer build_scenes(int nr_scenes, ...);
void destroy_scene(Scene* scene);

#endif // SCENE_LOADER_H_