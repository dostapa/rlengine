#include "../engine/object.h"
#include "../engine/scene.h"
#include "../engine/util.h"
#include <thread>

using namespace std;
using namespace dfo;

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#define GLSL_VERSION 330


//eneme stores object & passed, passed represents the goal-to-reach, in this case there are goals 0,1,2
struct eneme{
    object* obj;
    u16 passed;
};

enum TOWER_TYPE{
    TOWER_EMPTY = 0,
    TOWER_BASIC,
    TOWER_INTERMEDIATE,
    TOWER_ADVANCED
};

struct space{
    object* obj;
    TOWER_TYPE tt;
};

bool isUsed(space in){
    if(in.tt == TOWER_EMPTY){
        return true;
    }
    return false;
}

//Enemy spawn point
const Vector3 enemySpawn = {50,1,80};

//Object collections, easier reference. stores pointers to objects stored in Scene
vector<eneme> enemies;
vector<space> spaces;

vector<object*> goals;
vector<object*> path;
u16 health = 100;

float fov = 70.0f, sensitivity = 1.0f, width = 800, height= 800;
bool fullscreen = false;


int main() {
    //Initialization
    //Check for config.cfg in main directory. Extract width, height, fullscreen mode, FOV, and sens if they are defined.
    checkAndSetConfig(width, height, fullscreen, fov, sensitivity);
    //Initialize our OPENGL Context & open a window.
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow((int)width, (int)height, "Window");
    //Initialize our AUDIO context.
    InitAudioDevice();
    SetMasterVolume(100);

    Shader shader = LoadShader(TextFormat("../resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               TextFormat("../resources/shaders/glsl%i/fog.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    float fogDensity = 200.0f;
    int fogDensityLoc = GetShaderLocation(shader, "fogDensity");
    SetShaderValue(shader, fogDensityLoc, &fogDensity, SHADER_UNIFORM_FLOAT);

    // Ambient light level
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float arr[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(shader, ambientLoc, arr, SHADER_UNIFORM_VEC4);
    //Creates a new scene. Scenes are simply a vector of pointers to objects. Allows us to have a common draw function, easier cleanup, searching by ID
    auto* Scene = new scene(); // ->

    //Add to scene 'Path'
    path.push_back(Scene->add_to_scene("path","../resources/models/path1.obj",WHITE));
    path.push_back(Scene->add_to_scene("path","../resources/models/path2.obj",WHITE));
    path.push_back(Scene->add_to_scene("path","../resources/models/path3.obj",WHITE));
    //Set the scale and position of our new objects
    for(auto& piece : path){
        piece->set_scale({1,1,1});
        piece->set_position({50,0,50.5});
        piece->get_model().materials[0].shader = shader;
    }
    //Create our enemy goal points.
    goals.push_back(Scene->add_to_scene("goal","../resources/models/default.obj",WHITE));
    goals.push_back(Scene->add_to_scene("goal","../resources/models/default.obj",WHITE));
    goals.push_back(Scene->add_to_scene("goal-end","../resources/models/default.obj",WHITE));

    //Set goals along path for the enemy to follow.
    for(auto goal : goals){
        goal->set_scale({2,2,2});
        goal->get_model().materials[0].shader = shader;
        goal->draw = false;
    }
    goals.at(0)->set_position({50, 1, 21});
    goals.at(1)->set_position({109,1,21});
    goals.at(2)->set_position({109,1,-45});


    //Create our tower placement points

    //Camera to look at the game world
    Camera3D camera = {{60,70,100},{60,3,21.75},{0,1,0},fov, CAMERA_PERSPECTIVE};
    //Used to determine when to spawn enemies. When an enemy is spawned it is again set to GetTime()+5
    double enemyTimer = GetTime()+5;
    while(!WindowShouldClose()){
        //If the health is 0 or below, do not draw the game. Inform the player they lost.
        if(health<=0){
            BeginDrawing();
                ClearBackground(BLACK);
                DrawText("You Lost!", 0,height/2,50,WHITE);
            EndDrawing();
        }
        //If health is greater than 0, draw the game.
        else{
            //Check if it is time to spawn an enemy.
            if(GetTime() >= enemyTimer){
                //Create a new enemy object and add it to our scene.
                auto* new_enemy_obj = Scene->add_to_scene("Enemy", "../resources/models/cube.obj",WHITE);
                //Set scale and position
                new_enemy_obj->set_scale({0.5, 0.5, 0.5});
                new_enemy_obj->set_position(enemySpawn);
                new_enemy_obj->get_model().materials[0].shader = shader;
                //Create eneme structure that keeps track of current goal
                eneme new_enemy = {new_enemy_obj,0};
                //Add our newly created enemy to our enemies vector (allows us to keep all enemies in one place
                // - These are still linked to the scene and need to be deleted at the same time).
                enemies.push_back(new_enemy);
                //Increment timer
                enemyTimer = GetTime()+5.0f;
            }

            //Through enemies array
            for(int i = 0 ; i < enemies.size() ; i++){
                //Speed in which enemies will travel towards the goal
                float enemy_speed = 15.0f;
                //Pointer to the current enemy.
                eneme* enemy = &enemies.at(i);

                //Track next goal box
                if(enemy->obj->get_position().x < goals.at(enemy->passed)->get_position().x){
                    enemy->obj->translate({enemy_speed*delta_time,0,0});
                }
                else if(enemy->obj->get_position().x >= goals.at(enemy->passed)->get_position().x){
                    enemy->obj->translate({-enemy_speed*delta_time,0,0});
                }
                if(enemy->obj->get_position().z < goals.at(enemy->passed)->get_position().z){
                    enemy->obj->translate({0,0,enemy_speed*delta_time});
                }
                else if(enemy->obj->get_position().z >= goals.at(enemy->passed)->get_position().z){
                    enemy->obj->translate({0,0,-enemy_speed*delta_time});
                }

                //Increments passed iff the current goal reached is NOT the final goal (in this case, goal #2)
                if(colliding(enemy->obj,goals.at(enemy->passed)) && enemy->passed < 2){
                    enemy->passed = enemy->passed + 1;
                }
                //If the enemy has reached the final goal, destroy it and remove from scene. Decrease player health.
                else if (colliding(enemy->obj,goals.at(enemy->passed)) && enemy->passed == 2){
                    u16 id = enemy->obj->get_id();
                    enemies.erase(enemies.begin()+i);
                    Scene->remove_by_id(id);
                    health-=10;
                }
            }

            //UI text
            char fpsBuffer[5] = {0};
            sprintf(fpsBuffer,"%d",GetFPS());
            char healthbar[20] = {0};
            sprintf(healthbar,"HEALTH : %d",health);

            //Check for 'F11'
            fullscreenCheck(fullscreen,(int)width,(int)height);
            //Lock mouse within window borders
            bind_mouse();

            BeginDrawing();
                //Clear
                ClearBackground(SKYBLUE);
                //UI elements
                DrawText(fpsBuffer,5,5,35,BLACK);
                DrawText(healthbar,5,30,35,RED);

                BeginMode3D(camera);
                    //Draw our scene.
                    Scene->draw_scene();
                EndMode3D();

            EndDrawing();
        }

    }
    printf("TERMINATING\n");

    //Clear our collection vectors
    enemies.clear();
    goals.clear();
    spaces.clear();
    //Clear Scene , deletes all objects -> unloads models, texture, etc, from VRAM , RAM
    Scene->clear_scene();
    delete Scene;
    UnloadShader(shader);
    //Close our OPENGL Context
    CloseWindow();
    //Close our AUDIO Context
    CloseAudioDevice();

    //0
    exit(0);
}