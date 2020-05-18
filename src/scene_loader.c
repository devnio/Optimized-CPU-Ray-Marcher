#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "vec3.h"
#include "camera.h"
#include "light.h"
#include "material.h"

#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "geometry/box.h"
#include "geometry/cone.h"
#include "geometry/octahedron.h"
#include "geometry/torus.h"
#include "geometry/mandelbulb.h"
#include "scene_loader.h"

#define JSMN_STRICT
#include "jsmn.h"

#define SCENES_PATH "../scenes/"
#define JSON_PARSER_LOG_PATH "../scenes/log_parser.txt"

float _div_widthPx;
float _div_heightPx;

/*
Creates a scene container containing and empty array of nr_scenes scenes.
*/
SceneContainer create_scene_container(int nr_scenes)
{
    Scene **scenes = (Scene **)malloc(sizeof(Scene *) * nr_scenes);

    SceneContainer scene_container;
    scene_container.num_scenes = nr_scenes;
    scene_container.scenes = scenes;

    return scene_container;
}

/*
Adds a scene created from the scene file named scene_name,json into idx of the scene container 
*/
void add_scene(SceneContainer *scene_container, char *scene_name, int idx)
{
    scene_container->scenes[idx] = create_scene_from_json(scene_name);
}

void destroy_scene(Scene *scene)
{
    free(scene->name);
    free(scene->camera);

    free(scene->light->c);
    free(scene->light->emissionColor);
    free(scene->light);

    // free materials
    for (int i = 0; i < scene->nr_materials; i++)
    {
        free(scene->materials[i]);
    }
    free(scene->materials);

    // free transforms
    for (int i = 0; i < scene->nr_transforms; i++)
    {
        free(scene->transforms[i]->center); // TODO: check if this works
        free(scene->transforms[i]);
    }
    free(scene->transforms);

    // free geometric objects
    for (int i = 0; i < scene->nr_geom_objs; i++)
    {
        free(scene->geometric_ojects[i]->params);
        free(scene->geometric_ojects[i]);
    }
    free(scene->geometric_ojects);
}

//================================================
// JSON UTILITY
//================================================
/*
Prints the content of a token.
*/
void debug_token(char *json_str, jsmntok_t *tok, char *name_of_field)
{
    printf("[DEBUG TOKEN] %s: %.*s\n", name_of_field, tok->end - tok->start, json_str + tok->start);
}

/* 
Perform equality on token string and input s (for parsing).
*/
static int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return 0;
    }
    return -1;
}

/*
Parses an int.
Input: 
   - json string.
   - token containing int.
*/
static int get_int(const char *json_str, jsmntok_t *tok)
{
    return strtol(json_str + tok->start, NULL, 10);
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
   - logFile: pointer to logFile used to store information abount parsed scene
   - json string.
   - tokens.
   - index where the vector is (the block, not x).
*/
static void parse_vec3(FILE *logFile, char *json_str, jsmntok_t *tokens, int idx, double vec_res[NR_VEC_ELEMENTS])
{
    fprintf(logFile, "       +-- vector: ");

    int j = 1;
    for (int step = 0; step < tokens[idx].size; step++)
    {
        if (jsoneq(json_str, &tokens[idx + j], "x") == 0)
        {
            vec_res[0] = get_double(json_str, &tokens[idx + j + 1]);
            fprintf(logFile, "x: %f, ", vec_res[0]);
        }
        else if (jsoneq(json_str, &tokens[idx + j], "y") == 0)
        {
            vec_res[1] = get_double(json_str, &tokens[idx + j + 1]);
            fprintf(logFile, "y: %f, ", vec_res[1]);
        }
        else if (jsoneq(json_str, &tokens[idx + j], "z") == 0)
        {
            vec_res[2] = get_double(json_str, &tokens[idx + j + 1]);
            fprintf(logFile, "z: %f\n", vec_res[2]);
        }

        // jump onto next token holdin info at this hierarchy level
        j += 2;
    }

}

//================================================
// SCENE CREATION
//================================================
/*
Input:
 - scene: scene we are creating
 - logFile: pointer to logFile used to store information abount parsed scene
 - json string file
 - tokens: set of descriptors containing properties of the json file
 - idx: index for accessing the token containing info about camera (camera block info)
Output:
 - creates a camera and adds it to the scene, mallocs also the output image
 - how many tokens we moved (this is used then by the main loop to proceed to the next fields)
*/
int create_cam(Scene *scene, FILE *logFile, char *json_str, jsmntok_t *tokens, int idx_camera_properties)
{
    fprintf(logFile, "\n===================== CAMERA ====================\n");
    fprintf(logFile, "+-- parsing %d elements of camera.\n", tokens[idx_camera_properties].size);
    // we start with 1 because we have to step inside the tokens of camera
    // (0: the whole camera block, 1: fov, 2: value of fox, 3: position, 4:value and so on...)
    int width, height;
    double fov;
    double position[NR_VEC_ELEMENTS];
    double rotation[NR_VEC_ELEMENTS];

    int j = 1;
    if (jsoneq(json_str, &tokens[idx_camera_properties + j], "width") == 0)
    {
        width = get_int(json_str, &tokens[idx_camera_properties + j + 1]);
        fprintf(logFile, "+-- width: %d\n", width);
        j += tokens[idx_camera_properties + j].size + 1;
    }
    if (jsoneq(json_str, &tokens[idx_camera_properties + j], "height") == 0)
    {
        height = get_int(json_str, &tokens[idx_camera_properties + j + 1]);
        fprintf(logFile, "+-- height: %d\n", height);
        j += tokens[idx_camera_properties + j].size + 1;
    }
    if (jsoneq(json_str, &tokens[idx_camera_properties + j], "fov") == 0)
    {
        fov = get_double(json_str, &tokens[idx_camera_properties + j + 1]);
        fprintf(logFile, "+-- fov: %f\n", fov);
        j += tokens[idx_camera_properties + j].size + 1;
    }
    if (jsoneq(json_str, &tokens[idx_camera_properties + j], "position") == 0)
    {
        fprintf(logFile, "+-- position, size: %d\n", tokens[idx_camera_properties + j + 1].size);
        parse_vec3(logFile, json_str, tokens, idx_camera_properties + j + 1, position);
        j += tokens[idx_camera_properties + j + 1].size * 2 + 2;
    }
    if (jsoneq(json_str, &tokens[idx_camera_properties + j], "rotation") == 0)
    {
        fprintf(logFile, "+-- rotation, size: %d\n", tokens[idx_camera_properties + j + 1].size);
        parse_vec3(logFile, json_str, tokens, idx_camera_properties + j + 1, rotation);
        j += tokens[idx_camera_properties + j + 1].size * 2 + 2;
    }

    Camera *camera = create_camera(fov, width, height);
    move_camera(camera, position);
    rotate_camera(camera, rotation[0], rotation[1]);
    scene->camera = camera;
    
    // Assuming width and height of camera doesn't change
    _div_widthPx = 2 / (float) width; // pre-compute
    _div_heightPx = 2 / (float) height; // pre-compute

    return j;
}

/*
Input:
 - scene: scene we are creating
 - logFile: pointer to logFile used to store information abount parsed scene
 - json string file
 - tokens: set of descriptors containing properties of the json file
 - idx: index for accessing the token containing info about light (light block info)
Output:
 - how many tokens we moved (this is used then by the main loop to proceed to the next fields)
*/
int create_light(Scene *scene, FILE *logFile, char *json_str, jsmntok_t *tokens, int idx)
{
    fprintf(logFile, "\n===================== LIGHT =====================\n");
    fprintf(logFile, "+-- parsing %d elements of light.\n", tokens[idx].size);
    double center[NR_VEC_ELEMENTS];
    double emission[NR_VEC_ELEMENTS];

    int j = 1;

    if (jsoneq(json_str, &tokens[idx + j], "position") == 0)
    {
        fprintf(logFile, "+-- position, size: %d\n", tokens[idx + j + 1].size);
        parse_vec3(logFile, json_str, tokens, idx + j + 1, center);
        j += tokens[idx + j + 1].size * 2 + 2;
    }
    if (jsoneq(json_str, &tokens[idx + j], "emission") == 0)
    {
        fprintf(logFile, "+-- emission, size: %d\n", tokens[idx + j + 1].size);
        parse_vec3(logFile, json_str, tokens, idx + j + 1, emission);
        j += tokens[idx + j + 1].size * 2 + 2;
    }

    scene->light = create_pointlight(center, emission);

    return j;
}

/*
Input:
 - scene: scene we are creating
 - logFile: pointer to logFile used to store information abount parsed scene
 - json string file
 - tokens: set of descriptors containing properties of the json file
 - idx: index for accessing the token containing info about transforms (block info)
Output:
 - how many tokens we moved (this is used then by the main loop to proceed to the next fields)
*/
int create_transforms(Scene *scene, FILE *logFile, char *json_str, jsmntok_t *tokens, int idx)
{
    fprintf(logFile, "\n=================== TRANSFORMS ==================\n");
    fprintf(logFile, "+-- parsing %d transforms.\n", tokens[idx].size);

    scene->nr_transforms = tokens[idx].size;
    scene->transforms = (Transform **)malloc(sizeof(Transform *) * scene->nr_transforms);

    double pos[NR_VEC_ELEMENTS];
    double rot[NR_VEC_ELEMENTS];

    int j = 2;
    for (int step = 0; step < tokens[idx].size; step++)
    {
        // parse position and rotation immediately in 1 iteration (we know how a transform is composed)
        fprintf(logFile, "\n+--transform %d\n", step);
        if (jsoneq(json_str, &tokens[idx + j], "position") == 0)
        {
            fprintf(logFile, "   +-- position, size: %d\n", tokens[idx + j + 1].size);
            parse_vec3(logFile, json_str, tokens, idx + j + 1, pos);
            j += tokens[idx + j + 1].size * 2 + 2;
        }
        if (jsoneq(json_str, &tokens[idx + j], "rotation") == 0)
        {
            fprintf(logFile, "   +-- rotation, size: %d\n", tokens[idx + j + 1].size);
            parse_vec3(logFile, json_str, tokens, idx + j + 1, rot);
            j += tokens[idx + j + 1].size * 2 + 3;
        }

        scene->transforms[step] = new_transform(pos, rot);
    }
    return j - 1;
}

/*
Input:
 - scene: scene we are creating
 - logFile: pointer to logFile used to store information abount parsed scene
 - json string file
 - tokens: set of descriptors containing properties of the json file
 - idx: index for accessing the token containing info about materials (block info)
Output:
 - how many tokens we moved (this is used then by the main loop to proceed to the next fields)
*/
int create_materials(Scene *scene, FILE *logFile, char *json_str, jsmntok_t *tokens, int idx)
{
    fprintf(logFile, "\n=================== MATERIALS ===================\n");
    fprintf(logFile, "+-- parsing %d materials.\n", tokens[idx].size);

    scene->nr_materials = tokens[idx].size;
    scene->materials = (Material **)malloc(sizeof(Material *) * scene->nr_materials);

    double refl;
    double shininess;

    double surfaceCol[NR_VEC_ELEMENTS];

    int j = 2;
    for (int step = 0; step < tokens[idx].size; step++)
    {
        // parse refl, shininess, surface immediately in 1 iteration (we know how a transform is composed)
        fprintf(logFile, "\n+-- material %d\n", step);
        if (jsoneq(json_str, &tokens[idx + j], "reflection") == 0)
        {
            refl = get_double(json_str, &tokens[idx + j + 1]);
            fprintf(logFile, "   +-- reflection: %f\n", refl);
            j += 2;
        }
        if (jsoneq(json_str, &tokens[idx + j], "shininess") == 0)
        {
            shininess = get_double(json_str, &tokens[idx + j + 1]);
            fprintf(logFile, "   +-- shininess: %f\n", shininess);
            j += 2;
        }
        if (jsoneq(json_str, &tokens[idx + j], "surface") == 0)
        {
            fprintf(logFile, "   +-- surface, size: %d\n", tokens[idx + j + 1].size);
            parse_vec3(logFile, json_str, tokens, idx + j + 1, surfaceCol);
            j += tokens[idx + j + 1].size * 2 + 3;
        }

        scene->materials[step] = new_material(surfaceCol, refl, shininess);
    }
    return j - 1;
}

/*
Parses an array of doubles and saves into the input array.
Input: 
   - scene: scene we are creating
   - logFile: pointer to logFile used to store information abount parsed scene
   - json string.
   - tokens.
   - index where the vector is (the block, not x).
   - pointer to double array that is going to be filled.
*/
int create_params_array(Scene *scene, FILE *logFile, char *json_str, jsmntok_t *tokens, int idx, double *params, char* name)
{
    fprintf(logFile, "    +-- parsing %d params (array).\n", tokens[idx].size);
    int j = 1;
    fprintf(logFile, "        +-- ");
    for (int step = 0; step < tokens[idx].size; step++)
    {
        if (tokens[idx + j].type == JSMN_PRIMITIVE)
        {
            params[step] = get_double(json_str, &tokens[idx + j]);
            fprintf(logFile, "p[%d]: %f, ", step, params[step]);
        }
        j++;
    }

    // additional precomputation parameters
    if (strcmp(name, "cone") == 0)
    {   
        // k2
        params[3] = params[1] - params[0];
        params[4] = params[2] * 2.;

        // dot(k2,k2)
        params[5] = 1.0 / (params[3] * params[3] + params[4] * params[4]);
    }

    // additional precomputation parameters
    if (strcmp(name, "torus") == 0)
    {   
        params[0] =  params[0]*params[0];
    }

    return j + 1;
}

/*
Input:
 - scene: scene we are creating
 - logFile: pointer to logFile used to store information abount parsed scene
 - json string file
 - tokens: set of descriptors containing properties of the json file
 - idx: index for accessing the token containing info about geometric objects (block info)
Output:
 - how many tokens we moved (this is used then by the main loop to proceed to the next fields)
*/
int create_geom_objects(Scene *scene, FILE *logFile, char *json_str, jsmntok_t *tokens, int idx)
{
    fprintf(logFile, "\n============== GEOMETRIC OBJECTS ================\n");
    fprintf(logFile, "+-- parsing %d geometric objects.\n", tokens[idx].size);

    scene->nr_geom_objs = tokens[idx].size;
    scene->geometric_ojects = (GeomtericObject **)malloc(sizeof(GeomtericObject *) * scene->nr_geom_objs);

    int j = 2;
    for (int step = 0; step < tokens[idx].size; step++)
    {
        GeomtericObject *geom_obj = (GeomtericObject *)malloc(sizeof(GeomtericObject));

        // parse refl, shininess, surface immediately in 1 iteration (we know how a transform is composed)
        fprintf(logFile, "\n+-- object %d\n", step);
        char name[50];
        if (jsoneq(json_str, &tokens[idx + j], "name") == 0)
        {
            sprintf(name, "%.*s", tokens[idx + j + 1].end - tokens[idx + j + 1].start, json_str + tokens[idx + j + 1].start);
            fprintf(logFile, "   +-- %s: %s", "name", name);

            if (strcmp(name, "plane") == 0)
            {
                geom_obj->sdf = &sdf_plane;
                fprintf(logFile, " => assigned sdf of %s\n", "plane");
            }
            else if (strcmp(name, "sphere") == 0)
            {
                geom_obj->sdf = &sdf_sphere;
                fprintf(logFile, " => assigned sdf of %s\n", "sphere");
            }
            else if (strcmp(name, "box") == 0)
            {
                geom_obj->sdf = &sdf_box;
                fprintf(logFile, " => assigned sdf of %s\n", "box");
            }
            else if (strcmp(name, "cone") == 0)
            {
                // geom_obj->sdf = &sdf_cone;
                printf("TODO: VECTORIZE CONE");
                fflush(stdout);
                fprintf(logFile, " => assigned sdf of %s\n", "cone");
            }
            else if (strcmp(name, "octahedron") == 0)
            {
                geom_obj->sdf = &sdf_octahedron;
                // printf("TODO: VECTORIZE OCTAHEDRON");
                // fflush(stdout);
                fprintf(logFile, " => assigned sdf of %s\n", "octahedron");
            }
            else if (strcmp(name, "torus") == 0)
            {
                geom_obj->sdf = &sdf_torus;
                fprintf(logFile, " => assigned sdf of %s\n", "torus");
            }
            else if (strcmp(name, "mandelbulb") == 0)
            {
                // geom_obj->sdf = &sdf_mandelbulb;
                printf("TODO: VECTORIZE MANDELBULB");
                fflush(stdout);
                fprintf(logFile, " => assigned sdf of %s\n", "mandelbulb");
            }

            j += 2;
        }
        if (jsoneq(json_str, &tokens[idx + j], "transform_idx") == 0)
        {
            int transf_idx = get_int(json_str, &tokens[idx + j + 1]);
            geom_obj->transform = scene->transforms[transf_idx];

            fprintf(logFile, "   +-- transform_idx: %d\n", transf_idx);
            j += 2;
        }
        if (jsoneq(json_str, &tokens[idx + j], "material_idx") == 0)
        {
            int mat_idx = get_int(json_str, &tokens[idx + j + 1]);
            geom_obj->mat = scene->materials[mat_idx];

            fprintf(logFile, "   +-- material_idx: %d\n", mat_idx);
            j += 2;
        }
        if (jsoneq(json_str, &tokens[idx + j], "params") == 0)
        {
            int params_size = tokens[idx + j + 1].size;

            // check if this sdf has also constants that can be precomputed and inserted in params
            if (strcmp(name, "cone") == 0)
            {   
                params_size += 3; // precomp k1, k2, k22
            }

            double *params = (double *)malloc(sizeof(double) * params_size);
            j += create_params_array(scene, logFile, json_str, tokens, idx + j + 1, params, name) + 1;
            geom_obj->params = params;
        }

        scene->geometric_ojects[step] = geom_obj;
    }
    return j - 1;
}

//================================================
// JSON PARSING
//================================================
/*
Read json file and allocate the entire string in memory.
Make sure to free the buffer after using.
*/
static char *read_json(FILE *logFile, char *json_scene_path)
{
    char *buffer = 0;
    size_t length;
    FILE *f = fopen(json_scene_path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f) + 1;
        fseek(f, 0, SEEK_SET);
        buffer = (char *)malloc(length);

        if (buffer)
        {
            size_t r = fread(buffer, sizeof(char), length - 1, f);
            if (r == length - 1)
            {
                fprintf(logFile, "Successful read of json file \"%s\" into string variable.\n", json_scene_path);
                buffer[r] = '\0';
            }
            else
            {
                fprintf(logFile, "ERROR: couldn't read json file \"%s\" into string variable.\n", json_scene_path);
            }
        }
        fclose(f);
    }

    return buffer;
}

Scene *create_scene_from_json(char *scene_name)
{
    // open log file to write the process of loading current scene
    FILE *logFile = fopen(JSON_PARSER_LOG_PATH, "w");
    if (logFile == NULL)
    {
        // unable to open file hence exit
        printf("\nERROR: Unable to open parser log file '%s' file.\n", JSON_PARSER_LOG_PATH);
        printf("Please check whether file exists and you have write privilege.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(logFile, "=================================================\n");
    fprintf(logFile, "================ LOG PARSING SCENE ==============\n");
    // build current date and time to append to log file
    char date_[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(date_, sizeof(date_) - 1, "========  [Date: %d.%m.%Y - %H:%M:%S]  ========", t);
    fprintf(logFile, "%s \n", date_);

    // create scene path from name
    fprintf(logFile, "Creating scene path from scene_name: \"%s\" and scenes_path: \"%s\" .\n", scene_name, SCENES_PATH);
    char *json_scene_path = (char *)malloc(sizeof(char) * 300);
    strcpy(json_scene_path, SCENES_PATH);
    strcat(json_scene_path, scene_name);
    strcat(json_scene_path, ".json");

    fprintf(logFile, "Parsing file path: %s\n", json_scene_path);

    // read json file into string
    char *json_str = read_json(logFile, json_scene_path);
    if (json_str == NULL)
    {
        printf("ERROR: Parser couldn't read file at: %s\n", json_scene_path);
        fprintf(logFile, "ERROR: Parser couldn't read file at: %s\n", json_scene_path);
        exit(EXIT_FAILURE);
    }

    // DEBUG
    fprintf(logFile, "Read file contains: \n_________________________\n%s\n_________________________\n", json_str);
    // DEBUG

    // initialize json parser
    jsmn_parser parser;
    jsmn_init(&parser);

    // get needed tokens
    int needed_tokens = jsmn_parse(&parser, json_str, strlen(json_str), NULL, -1);
    fprintf(logFile, "Amount of needed tokes are: %d\n", needed_tokens);

    // parse with newly created tokens
    jsmntok_t tokens[needed_tokens]; /* We expect no more than needed_tokens JSON tokens */
    jsmn_init(&parser);
    int r = jsmn_parse(&parser, json_str, strlen(json_str), tokens, needed_tokens);

    // check that json is in right format
    if (r < 0)
    {
        printf("ERROR: Parser failed to parse JSON: Return error r:%d\n", r);
        fprintf(logFile, "ERROR: Parser to parse JSON: Return error r:%d\n", r);
        exit(EXIT_FAILURE);
    }

    // check that the top-level element is an object
    if (r < 1 || tokens[0].type != JSMN_OBJECT)
    {
        printf("ERROR: Parser expected and object at root, probably forgot outer brackets. Return error r:%d\n", r);
        fprintf(logFile, "ERROR: Parser expected and object at root, probably forgot outer brackets. Return error r:%d\n", r);
        exit(EXIT_FAILURE);
    }

    // FILE WAS PARSED SUCCESSFULLY
    fprintf(logFile, "PARSING STATUS: SUCCESS!\n");
    Scene *scene = (Scene *)malloc(sizeof(Scene));
    scene->name = strdup(scene_name);

    // Now go through the objects and populate the scene
    for (int i = 1; i < r; i++)
    {
        if (jsoneq(json_str, &tokens[i], "camera") == 0)
        {
            i += create_cam(scene, logFile, json_str, tokens, i + 1) + 1;
        }
        if (jsoneq(json_str, &tokens[i], "pointlight") == 0)
        {
            i += create_light(scene, logFile, json_str, tokens, i + 1) + 1;
        }
        if (jsoneq(json_str, &tokens[i], "transforms") == 0)
        {
            i += create_transforms(scene, logFile, json_str, tokens, i + 1) + 1;
        }
        if (jsoneq(json_str, &tokens[i], "materials") == 0)
        {
            i += create_materials(scene, logFile, json_str, tokens, i + 1) + 1;
        }
        if (jsoneq(json_str, &tokens[i], "geometric_objects") == 0)
        {
            i += create_geom_objects(scene, logFile, json_str, tokens, i + 1) + 1;
        }
    }

    // close
    fprintf(logFile, "\n===================== END =======================\n\n\n\n\n\n");
    fclose(logFile);

    // free
    free(json_scene_path);
    free(json_str);

    return scene;
}
