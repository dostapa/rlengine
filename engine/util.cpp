//
// Created by soup99 on 2024-03-27.
//


#define gravity (-0.0097)
#include "util.h"

using namespace dfo;

void dfo::fullscreenCheck(bool &flag, int width, int height){
    if(!flag){
        SetWindowSize(width,height);
        ClearWindowState(FLAG_FULLSCREEN_MODE);

    }
    else {
        SetWindowSize(GetMonitorWidth(0),GetMonitorHeight(0));
        SetWindowState(FLAG_FULLSCREEN_MODE);

    }
    //Check for 'F11'
    if(!flag && IsKeyPressed(KEY_F11)) flag = true;
    else if (flag && IsKeyPressed(KEY_F11)) flag = false;
}
void dfo::checkAndSetConfig(float &w, float &h, bool &full, float &fov, float& sens){
    int number_of_lines = 0;
    std::string line;
    std::ifstream cfg("../config.cfg");
    while(!cfg.is_open());
    while (std::getline(cfg, line))
        ++number_of_lines;
    cfg.close();
    cfg.open("../config.cfg", std::ifstream::in);
    for(int i = 0 ; i<number_of_lines;i++) {
        std::getline(cfg, line);
        if (line == "FULLSCREEN=0"){
            full = false;
            continue;
        }
        else if (line == "FULLSCREEN=1") {
            full = true;
            fullscreenCheck(full,w,h);
            continue;
        }
        if (line.substr(0, 5) == "WIDTH") {
            int newWidth = stoi(line.substr(6));
            w = newWidth;
            continue;
        }
        else if (line.substr(0, 6) == "HEIGHT") {
            int newHeight = stoi(line.substr(7));
            h = newHeight;
            continue;
        }
        if(line.substr(0,3) == "FOV"){
            float newFov = stof(line.substr(4));
            fov = newFov;
            continue;
        }
        if(line.substr(0,4) == "SENS"){
            float newSens = stof(line.substr(5));
            sens = newSens * pow(10,3);
            printf("sensitivity set to %f", sens);
            continue;
        }
    }
    cfg.close();
}
void dfo::playerControls(Camera &player, object* physPlayer, object* surface, float speed, float sensitivity){
    physPlayer->set_position({player.position.x, physPlayer->get_position().y, player.position.z});
    player.position = {physPlayer->get_position().x, 3.5, physPlayer->get_position().z};
    UpdateCameraPro(&player,
                    (Vector3){
                            (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*speed*delta_time -      // Move forward-backward
                            (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*speed*delta_time,
                            (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*speed*delta_time -   // Move right-left
                            (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*speed*delta_time,
                            0.0f                                                // Move up-down
                    },
                    (Vector3){
                            GetMouseDelta().x*sensitivity*delta_time,                            // Rotation: yaw
                            GetMouseDelta().y*sensitivity*delta_time,                            // Rotation: pitch
                            0.0f                                                // Rotation: roll
                    },
                    0.0f);
    if(!colliding(physPlayer,surface)){
        physPlayer->translate({0,gravity,0});
        player.position = Vector3Subtract(physPlayer->get_position(),{0,-3.5,0});
        if(physPlayer->get_position().y <= -100){
            physPlayer->set_position({0,50,0});
            player.position =Vector3Subtract(physPlayer->get_position(),{0,-3.5,0});
        }
    }

}
bool dfo::colliding(object *in1, object *in2) {
    return CheckCollisionBoxes(in1->get_collider(),in2->get_collider());
}
