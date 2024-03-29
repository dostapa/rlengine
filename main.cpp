#include "engine/object.h"
#include "engine/scene.h"
#include "engine/util.h"


using namespace std;
using namespace dfo;

int main() {


    float fov = 70.0f, sensitivity = 1.0f, width = 800, height= 800;
    bool fullscreen = false;
    checkAndSetConfig(width, height, fullscreen, fov, sensitivity);

    char **tags = new char *[max_tags];
    for (int i = 0; i < max_tags; ++i) {
        tags[i] = new char[named_size];
    }
    tags[0] = (char *) "Player";
    tags[1] = (char *) "Generic";
    tags[2] = (char *) "Ground";
    tags[3] = (char *) "Enemy";
    tags[4] = (char *) "Liquid";
    tags[5] = (char *) "UI";
    tags[6] = (char *) "Controllable";
    tags[7] = (char *) "Custom";
    tags[8] = (char *) "Custom";
    tags[9] = (char *) "Custom";

    InitWindow(width, height, "Window");
    InitAudioDevice();
    SetMasterVolume(100);
    Music fartnuts = LoadMusicStream("../resources/audio/poopfartnuts.mp3");
    PlayMusicStream(fartnuts);
    scene scene1;
    SetWindowFocused();

    object *player = scene1.add_to_scene(
            "Player",
            "../resources/models/player.obj",
            tags,
            1,
            WHITE
            );
    player->set_position({0, 10, 0});

    object *ground = scene1.add_to_scene(
            "Ground",
            "../resources/models/cube.obj",
            &tags[2],
            1,
            WHITE
    );
    ground->set_scale({50, 5, 50});
    ground->set_position({0, -5, 0});

    Camera3D camera = {0};
    camera.target = {0, 0, 0};
    camera.position = { -15,3.5,-15};
    camera.fovy = fov;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.up = {0,1,0};

    while(!WindowShouldClose()){
        int fps = GetFPS();
        char buf[5] = {0};
        itoa(fps,buf,10);
        fullscreenCheck(fullscreen,width,height);
        playerControls(camera,player, ground, (IsKeyDown(KEY_LEFT_SHIFT)) ? 14.50f : 7.25, sensitivity);
        if(GetMouseX() < 5){
            if(GetMouseY() < 5)
                SetMousePosition(6,6);
            else
                SetMousePosition(6,GetMouseY());
        }
        else if(GetMouseY() < 5) {
            SetMousePosition(GetMouseX(), 6);
        }
        else if(GetMouseX() > GetScreenWidth()-9){
            if(GetMouseY() > GetScreenHeight()-9)
                SetMousePosition(GetScreenWidth()-10,GetScreenHeight()-10);
            SetMousePosition(GetScreenWidth()-10,GetMouseY());
        }
        else if(GetMouseY() > GetScreenHeight()-9) {
            SetMousePosition(GetMouseX(), GetScreenHeight()-10);
        }
        BeginDrawing();
        ClearBackground(SKYBLUE);
            BeginMode3D(camera);
                scene1.draw_scene();
                if(IsKeyPressed(KEY_J)){
                    object * thingy = scene1.add_to_scene(
                            "Cube",
                            "../models/cube.obj",
                            &tags[1],
                            1,
                            GREEN
                            );
                    thingy->set_position({(float)GetRandomValue(0, 255), (float)GetRandomValue(0, 255), (float)GetRandomValue(0, 255)});
                    thingy->set_scale({5,5,5});
                }
            EndMode3D();
        DrawText(buf,5,5,35,PURPLE);
        EndDrawing();
    }
    CloseWindow();

    scene1.clear_scene();
    delete[] tags;
    exit(0);
}