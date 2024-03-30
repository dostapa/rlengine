#include "../engine/object.h"
#include "../engine/scene.h"
#include "../engine/util.h"
#include <thread>

using namespace std;
using namespace dfo;
const Vector3 enemySpawn = {50,1,50};
const Vector3 enemyGoal = {64,1, 50.25};
vector<object*> enemies;

int main() {
    float fov = 70.0f, sensitivity = 1.0f, width = 800, height= 800;
    bool fullscreen = false;
    checkAndSetConfig(width, height, fullscreen, fov, sensitivity);

    InitWindow((int)width, (int)height, "Window");
    InitAudioDevice();
    SetMasterVolume(100);
    //Music fartnuts = LoadMusicStream("../resources/audio/poopfartnuts.mp3");
    auto* Scene = new scene(); // ->
    auto* path = Scene->add_to_scene("path","../resources/models/pathone.obj",WHITE);
    auto* end = Scene->add_to_scene("end_point","../resources/models/default.obj",WHITE);
    Texture2D texture = LoadTexture("../resources/check.png");
    GenTextureMipmaps(&texture);
    end->set_position(enemyGoal);
    end->set_scale({0.5f,0.5f,0.5f});
    SetTextureWrap(texture,TEXTURE_WRAP_MIRROR_CLAMP);
    SetMaterialTexture(&end->get_model().materials[0], MATERIAL_MAP_DIFFUSE, texture);
    path->set_position({50,0,50.5});
    path->set_scale({1,1,1});
    Camera3D camera = {0};
    camera.target = enemySpawn;
    camera.position = { 50,20,40};
    camera.fovy = fov;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.up = {0,1,0};
    //PlayMusicStream(fartnuts);

    double enemyTimer = GetTime()+5;
    BoundingBox path_collider = path->get_collider();

    while(!WindowShouldClose()){
        if(GetTime() >= enemyTimer){
            object* newEnemy = Scene->add_to_scene("Enemy", "../resources/models/cube.obj",WHITE);
            newEnemy->set_scale({0.5, 0.5, 0.5});
            newEnemy->set_position(enemySpawn);
            enemies.push_back(newEnemy);
            enemyTimer = GetTime()+5;
        }

        for(auto enemy : enemies){
            if(enemy->get_position().x < enemyGoal.x){
                enemy->translate({2*delta_time,0,0});
            }
        }

        for (int i = 0; i < enemies.size(); ++i) {
            if(colliding(enemies.at(i),end)){
                object* t = enemies.back();
                enemies.back() = enemies.at(i);
                enemies.at(i) = t;
                u16 id = enemies.back()->get_id();
                enemies.pop_back();
                Scene->remove_by_id(id);
            }
        }

        //UpdateMusicStream(fartnuts);
        char fpsBuffer[5] = {0};
        sprintf(fpsBuffer,"%d",GetFPS());
        fullscreenCheck(fullscreen,(int)width,(int)height);
        bind_mouse();

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawText(fpsBuffer,5,5,35,BLACK);

            BeginMode3D(camera);
                Scene->draw_scene();
            EndMode3D();
        EndDrawing();
    }
    printf("\bQUITTING\n\n");
    CloseWindow();
    CloseAudioDevice();
    enemies.clear();
    Scene->clear_scene();
    delete Scene;

    exit(0);
}