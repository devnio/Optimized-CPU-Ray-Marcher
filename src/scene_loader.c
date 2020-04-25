#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "vec3.h"
#include "camera.h"
#include "material.h"

#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "geometry/box.h"
#include "geometry/cone.h"
#include "geometry/octahedron.h"
#include "geometry/torus.h"
#include "scene_loader.h"

#include "jsmn.h"

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

//================================================
// JSON UTILITY
//================================================
void debug_token(char* json_str, jsmntok_t* tok, char* name_of_field) {
    printf("- %s: %.*s\n", name_of_field, tok->end - tok->start, json_str + tok->start);
}

/* 
Perform equality on token string and input s (for parsing).
*/
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

/*
Parses a double.
Input: 
   - json string.
   - token containing double.
*/
static double get_double(const char *json_str, jsmntok_t *tok)
{
    return strtod(json_str + tok->start, NULL);
}

/*
Parses a vector.
Input: 
   - json string.
   - tokens.
   - index where the vector is (the block, not x).
*/
static Vec3 parse_vec3(char* json_str, jsmntok_t* tokens, int idx)
{   
    printf("       +-- vector: ");
    Vec3 v;
    int j = 1;
    for (int step=0; step < tokens[idx].size; step++)
    {
        if(jsoneq(json_str, &tokens[idx+j], "x") == 0)
        {
            v.x = get_double(json_str, &tokens[idx + j + 1]);
            printf("x: %f, ", v.x);
        }
        else if(jsoneq(json_str, &tokens[idx+j], "y") == 0)
        {
            v.y = get_double(json_str, &tokens[idx + j + 1]);
            printf("y: %f, ", v.y);
        }
        else if(jsoneq(json_str, &tokens[idx+j], "z") == 0)
        {
            v.z = get_double(json_str, &tokens[idx + j + 1]);
            printf("z: %f\n", v.z);
        }

        // jump onto next token holdin info at this hierarchy level
        j+=2;
    }

    return v;
}

//================================================
// SCENE CREATION
//================================================
/*
Input:
 - json string file
 - tokens: set of descriptors containing properties of the json file
 - idx: index for accessing the token containing info about camera (camera block info)
Output:
 - how many tokens we moved (this is used then by the main loop to proceed to the next fields)
*/
int create_cam(char* json_str, jsmntok_t* tokens, int idx_camera_properties)
{
    printf("\nCAMERA\n");
    printf("+-- parsing %d elements of camera.\n", tokens[idx_camera_properties].size);
    // we start with 2 because we have to step inside the tokens of camera 
    // (0: the whole camera block, 1: fov, 2: value of fox, 3: position, 4:value and so on...)
    int j = 1;
    for(int step=0; step < tokens[idx_camera_properties].size; step++)
    {
        if(jsoneq(json_str, &tokens[idx_camera_properties+j], "fov") == 0)
        {
            double nr = strtod(json_str + tokens[idx_camera_properties + j + 1].start, NULL);
            printf("+-- fov: %f\n", nr);
            j+=tokens[idx_camera_properties+j].size+1;
        }
        else if(jsoneq(json_str, &tokens[idx_camera_properties+j], "position") == 0)
        {
            printf("+-- position, size: %d\n",tokens[idx_camera_properties+j+1].size);
            parse_vec3(json_str, tokens, idx_camera_properties+j+1);
            j+=tokens[idx_camera_properties+j+1].size*2+2;
        }
        else if(jsoneq(json_str, &tokens[idx_camera_properties+j], "rotation") == 0)
        {
            printf("+-- rotation, size: %d\n",tokens[idx_camera_properties+j+1].size);
            parse_vec3(json_str, tokens, idx_camera_properties+j+1);
            j+=tokens[idx_camera_properties+j+1].size*2+2;
        }
    }
    return j;
}

/*
Input:
 - json string file
 - tokens: set of descriptors containing properties of the json file
 - idx: index for accessing the token containing info about light (light block info)
Output:
 - how many tokens we moved (this is used then by the main loop to proceed to the next fields)
*/
int create_light(char* json_str, jsmntok_t* tokens, int idx)
{
    printf("\nLIGHT\n");
    printf("+-- parsing %d elements of light.\n", tokens[idx].size);
    int j = 1;
    for(int step=0; step < tokens[idx].size; step++)
    {
        if(jsoneq(json_str, &tokens[idx+j], "power") == 0)
        {
            double nr = strtod(json_str + tokens[idx + j + 1].start, NULL);
            printf("+-- power: %f\n", nr);
            j+=tokens[idx+j].size+1;
        }
        else if(jsoneq(json_str, &tokens[idx+j], "position") == 0)
        {
            printf("+-- position, size: %d\n",tokens[idx+j+1].size);
            parse_vec3(json_str, tokens, idx+j+1);
            j+=tokens[idx+j+1].size*2+2;
        }
    }
    return j;
}

/*
Input:
 - json string file
 - tokens: set of descriptors containing properties of the json file
 - idx: index for accessing the token containing info about transforms (block info)
Output:
 - how many tokens we moved (this is used then by the main loop to proceed to the next fields)
*/
int create_transforms(char* json_str, jsmntok_t* tokens, int idx)
{
    printf("\nTRANSFORMS\n");
    printf("+-- parsing %d transforms.\n", tokens[idx].size);
    int j = 2;
    for(int step=0; step < tokens[idx].size; step++)
    {
        // parse position and rotation immediately in 1 iteration (we know how a transform is composed)
        printf("\n+--transform %d\n", step);
        if(jsoneq(json_str, &tokens[idx+j], "position") == 0)
        {
            printf("   +-- position, size: %d\n",tokens[idx+j+1].size);
            parse_vec3(json_str, tokens, idx+j+1);
            j+=tokens[idx+j+1].size*2+2;
        }
        if(jsoneq(json_str, &tokens[idx+j], "rotation") == 0)
        {
            printf("   +-- rotation, size: %d\n",tokens[idx+j+1].size);
            parse_vec3(json_str, tokens, idx+j+1);
            j+=tokens[idx+j+1].size*2+3;
        }
    }
    return j-1;
}

/*
Input:
 - json string file
 - tokens: set of descriptors containing properties of the json file
 - idx: index for accessing the token containing info about materials (block info)
Output:
 - how many tokens we moved (this is used then by the main loop to proceed to the next fields)
*/
int create_materials(char* json_str, jsmntok_t* tokens, int idx)
{
    printf("\nMATERIALS\n");
    printf("+-- parsing %d materials.\n", tokens[idx].size);
    int j = 2;
    for(int step=0; step < tokens[idx].size; step++)
    {
        // parse refl, shininess, surface immediately in 1 iteration (we know how a transform is composed)
        printf("+-- material %d\n", step);
        if(jsoneq(json_str, &tokens[idx+j], "reflection") == 0)
        {
            double refl = get_double(json_str, &tokens[idx+j+1]);
            printf("   +-- reflection: %f\n", refl);
            j+=2;
        }
        if(jsoneq(json_str, &tokens[idx+j], "shininess") == 0)
        {
            double shininess = get_double(json_str, &tokens[idx+j+1]);
            printf("   +-- shininess: %f\n", shininess);
            j+=2;
        }
        if(jsoneq(json_str, &tokens[idx+j], "surface") == 0)
        {
            printf("   +-- surface, size: %d\n",tokens[idx+j+1].size);
            parse_vec3(json_str, tokens, idx+j+1);
            j+=tokens[idx+j+1].size*2+3;
        }
    }
    return j-1;
}

//================================================
// JSON PARSING
//================================================
/*
Read json file and allocate the entire string in memory.
Make sure to free the buffer after using.
*/
static char* read_json(char* json_scene_path) {
    char * buffer = 0;
    long length;
    FILE * f = fopen (json_scene_path, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose (f);
    }

    if (buffer)
    {
    // start to process your data / extract strings here...
        return buffer;
    }
    return NULL;
}

Scene* create_scene(char* json_scene_path)
{
    printf("\n=======================\nJSON PARSING STARTED\n");
    printf("Parsing file at: %s\n", json_scene_path);
    // read json file into string
    char* json_str = read_json(json_scene_path);
    if (json_str == NULL)
    {
        printf("Couldn't read file at: %s\n", json_scene_path);
    }

    // initialize json parser
    jsmn_parser parser;
    jsmn_init(&parser);

    // get needed tokens 
    int needed_tokens = jsmn_parse(&parser, json_str, strlen(json_str), NULL, -1);
    printf("Amount of needed tokes are: %d\n", needed_tokens);
    
    // parse with newly created tokens
    jsmntok_t tokens[needed_tokens]; /* We expect no more than needed_tokens JSON tokens */
    jsmn_init(&parser);
    int r = jsmn_parse(&parser, json_str, strlen(json_str), tokens, needed_tokens);

    // check that json is in right format
    if (r < 0) {
        printf("Failed to parse JSON: Return error r:%d\n", r);
    }

    // check that the top-level element is an object 
    if (r < 1 || tokens[0].type != JSMN_OBJECT) {
        printf("Object expected, probably forgot outer brackets. Return error r:%d\n", r);
    }

    // FILE WAS PARSED SUCCESSFULLY
    // Create scene
    // Scene* scene = 

    // Now go through the objects and populate the scene
    for (int i = 1; i < r; i++) 
    {
        if (jsoneq(json_str, &tokens[i], "camera") == 0) {    
            // debug_token(json_str, &tokens[i+1], "camera");
            i += create_cam(json_str, tokens, i+1) + 1;
        }
        if(jsoneq(json_str, &tokens[i], "pointlight") == 0)
        {
            i += create_light(json_str, tokens, i+1) + 1;
        }
        if(jsoneq(json_str, &tokens[i], "transforms") == 0)
        {
            i += create_transforms(json_str, tokens, i+1) + 1;
        }
        if(jsoneq(json_str, &tokens[i], "materials") == 0)
        {
            i += create_materials(json_str, tokens, i+1) + 1;
            debug_token(json_str, &tokens[i], "next");
        }
        if(jsoneq(json_str, &tokens[i], "materials") == 0)
        {
            i += create_materials(json_str, tokens, i+1) + 1;
            debug_token(json_str, &tokens[i], "next");
        }
    }

    return NULL;
}


