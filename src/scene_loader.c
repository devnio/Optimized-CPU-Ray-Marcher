#include "scene_loader.h"
#include <stdlib.h>
#include <stdarg.h>

#define MAX_NR_SCENES 10
int num_scenes = 0;
scene_builder scenes_builder[MAX_NR_SCENES];

void add_plane(int nr, Plane** sps, ...){
    va_list arguments;                     
    va_start ( arguments, sps);           
    for ( int x = 0; x < nr; x++ )        
    {
        sps[x] = va_arg ( arguments, Plane* ); 
    }
    va_end ( arguments );  
}
void add_spheres(int nr, Sphere** sps, ...){
    va_list arguments;                     
    va_start ( arguments, sps);           
    for ( int x = 0; x < nr; x++ )        
    {
        sps[x] = va_arg ( arguments, Sphere* ); 
    }
    va_end ( arguments );  
}
void add_octahedrons(int nr, Octahedron** sps, ...){
    va_list arguments;                     
    va_start ( arguments, sps);           
    for ( int x = 0; x < nr; x++ )        
    {
        sps[x] = va_arg ( arguments, Octahedron* ); 
    }
    va_end ( arguments );  
}
void add_boxes(int nr, Box** sps, ...){
    va_list arguments;                     
    va_start ( arguments, sps);           
    for ( int x = 0; x < nr; x++ )        
    {
        sps[x] = va_arg ( arguments, Box* ); 
    }
    va_end ( arguments );  
}
void add_cones(int nr, Cone** sps, ...){
    va_list arguments;                     
    va_start ( arguments, sps);           
    for ( int x = 0; x < nr; x++ )        
    {
        sps[x] = va_arg ( arguments, Cone* ); 
    }
    va_end ( arguments );  

}
void add_torus(int nr, Torus** tos, ...){
    va_list arguments;                     
    va_start ( arguments, tos);           
    for ( int x = 0; x < nr; x++ )        
    {
        tos[x] = va_arg ( arguments, Torus* ); 
    }
    va_end ( arguments );  

}
////////////////////////////////  SCENES DEFINITION /////////////////////////////////////

Scene* scene_baseline(){

    PointLight pLight;
    pLight.c = new_vector(0, 100, 0);
    double em = 2;
    pLight.emissionColor = new_vector(em, em, em);

    Scene* scene = build_scene(1,4,1,3,1,0, pLight, "../output/baseline.png");
  
    Plane* pl0 = build_plane(new_vector(0,1,0), 3, new_material(new_vector(0, 0.3, 0.6), 0, 15, new_vector(0, 0, 0)));
    add_plane(scene->nr_planes, scene->planes,pl0);
    Octahedron* oct0 = build_octahedron(new_vector(4, 0, 20),2,new_material(new_vector(0, 0.3, 0.6),0,15,new_vector(0, 0, 0)));
    add_octahedrons(scene->nr_octahedrons, scene->octahedrons, oct0);
    Sphere* sp0 = build_sphere(new_vector(50, 0, 100),50,new_material(new_vector(0.6, 0.6, 0),0,15,new_vector(0, 0, 0)));
    Sphere* sp1 = build_sphere(new_vector(4, 0, 25),3,new_material(new_vector(0.8, 0.1, 0), 0.3, 5, new_vector(0, 0, 0)));  
    Sphere* sp2 = build_sphere(new_vector(-4, 0, 15),3,new_material(new_vector(0.3, 1, 0.36),0,105,new_vector(0, 0, 0)));   
    Sphere* sp3 = build_sphere(new_vector(0, 0, 40),3, new_material(new_vector(0.2, 0.2, 0.97),0.1,15,new_vector(0, 0, 0)));
    add_spheres(scene->nr_spheres, scene->spheres, sp0, sp1, sp2, sp3);
    Box* box0 = build_box(new_vector(1.0,1.0,1.0), new_material(new_vector(0.3, 1, 0.36),0,105,new_vector(0, 0, 0)));
    add_boxes(scene->nr_boxes, scene->boxes, box0);
    Cone* cone0 = build_cone(new_vector(4, 3, 25),3,0.01,3,new_material(new_vector(0.8, 0.1, 0),0.1,5,new_vector(0, 0, 0)));
    Cone* cone1 = build_cone(new_vector(-4, 3, 15),3,0.01,3,new_material(new_vector(0.3, 1, 0.36),0.1,105,new_vector(0, 0, 0)));
    Cone* cone2 = build_cone(new_vector(0, 3, 40),3,0.01,3,new_material(new_vector(0.2, 0.2, 0.97),0.1,15,new_vector(0, 0, 0)));
    add_cones(scene->nr_cones, scene->cones,cone0, cone1, cone2);

    return scene;
}

Scene* scene_baseline2(){

    PointLight pLight;
    pLight.c = new_vector(0, 100, 0);
    double em = 2;
    pLight.emissionColor = new_vector(em, em, em);

    Scene* scene = build_scene(1,1,0,0,1,0, pLight, "../output/baseline2.png");
    
    Plane* pl0 = build_plane(new_vector(0,1,0), 3, new_material(new_vector(0, 0.3, 0.6), 0, 15, new_vector(0, 0, 0)));
    add_plane(scene->nr_planes, scene->planes,pl0);
    Octahedron* oct0 = build_octahedron(new_vector(4, 0, 20),2,new_material(new_vector(0, 0.3, 0.6),0,15,new_vector(0, 0, 0)));
    add_octahedrons(scene->nr_octahedrons, scene->octahedrons, oct0);
    Sphere* sp0 = build_sphere(new_vector(50, 0, 100),50,new_material(new_vector(0.6, 0.6, 0),0,15,new_vector(0, 0, 0)));
    add_spheres(scene->nr_spheres, scene->spheres, sp0);

    return scene;
}


Scene* scene_torus_test(){

    PointLight pLight;
    pLight.c = new_vector(0, 100, 0);
    double em = 2;
    pLight.emissionColor = new_vector(em, em, em);

    Scene* scene = build_scene(1,1,0,0,0,1, pLight, "../output/torus_test.png");
    
    Plane* pl0 = build_plane(new_vector(0,1,0), 3, new_material(new_vector(0, 0.3, 0.6), 0, 15, new_vector(0, 0, 0)));
    add_plane(scene->nr_planes, scene->planes,pl0);

    Torus* torus0 = build_torus(new_vector(0, 0, 6), 3.2, 0.7, new_material(new_vector(0.65, 0.2, 0.3),0,15,new_vector(0, 0, 0)));
    add_torus(scene->nr_toruses, scene->toruses, torus0);
    
    Sphere* sp0 = build_sphere(new_vector(50, 0, 100),50,new_material(new_vector(0.6, 0.6, 0),0,15,new_vector(0, 0, 0)));
    add_spheres(scene->nr_spheres, scene->spheres, sp0);

    return scene;
}


/*
*   TODO: here you can add the scenes that have to be added
*   Note: If needed modify MAX_NR_SCENES
*/
void add_scenes(){
    // add_scene(&scene_baseline2);
    add_scene(&scene_baseline);
    // add_scene(&scene_torus_test);
    
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
        scenes[i] = scenes_builder[i]();
    }
    SceneContainer scene_container;
    scene_container.num_scenes=num_scenes;
    scene_container.scenes=scenes;

    return scene_container;
}

void destroy_scene(Scene* scene){
    for(int i=0; i<scene->nr_planes;++i){
        Plane* p = (scene->planes)[i];
        free(p);
    }
    free(scene->planes);
    scene->planes=0;

    for(int i=0; i<scene->nr_spheres;++i){
        Sphere* s = (scene->spheres)[i];
        free(s);
    }
    free(scene->spheres);
    scene->spheres=0;

    for(int i=0; i<scene->nr_octahedrons;++i){
        Octahedron* o = (scene->octahedrons)[i];
        free(o);
    }
    free(scene->octahedrons);
    scene->octahedrons=0;

    for(int i=0; i<scene->nr_boxes;++i){
        Box* b = (scene->boxes)[i];
        free(b);
    }
    free(scene->boxes);
    scene->boxes=0;

    for(int i=0; i<scene->nr_cones;++i){
        Cone* c = (scene->cones)[i];
        free(c);
    }
    free(scene->cones);
    scene->cones=0;

    for(int i=0; i<scene->nr_toruses;++i){
        Torus* t = (scene->toruses)[i];
        free(t);
    }
    free(scene->toruses);
    scene->toruses=0;

    free(scene);
    scene=0;

}

