#ifndef SCENE_H_
    #include "geometry/scene.h"
#endif

typedef Scene*(*scene_builder)();

typedef struct {
    int num_scenes;
    Scene **scenes;
} SceneContainer;

void add_scene(scene_builder builder);
void add_scenes();
SceneContainer build_scenes();
Scene* scene_baseline();
void destroy_scene(Scene* scene);
