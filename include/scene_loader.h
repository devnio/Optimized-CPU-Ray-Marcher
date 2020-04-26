#ifndef SCENE_LOADER_H_
#define SCENE_LOADER_H_

#include "geometry/scene.h"

typedef struct
{
    int num_scenes;
    Scene **scenes;
} SceneContainer;

Scene *create_scene_from_json(char *scene_name);
SceneContainer create_scene_container(int nr_scenes);
void add_scene(SceneContainer *scene_container, char *scene_name, int idx);
void destroy_scene(Scene *scene);

#endif // SCENE_LOADER_H_