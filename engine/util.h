//
// Created by soup99 on 2024-03-27.
//

#ifndef SKADOOSH_UTIL_H
#define SKADOOSH_UTIL_H
#include "common.h"
#include "object.h"
namespace dfo{
    void bind_mouse();
    bool colliding(object* in1, object* in2);
    bool grounded(object* ground, object* groundee);
    void checkAndSetConfig(float &w, float &h, bool &full, float &fov, float& sens);
    void fullscreenCheck(bool &flag, int width, int height);
    void playerControls(Camera &player, object* physPlayer, object* surface, float speed, float sensitivity);
}


#endif //SKADOOSH_UTIL_H
