//
// Created by soup99 on 2024-03-27.
//

#ifndef SKADOOSH_COMMON_H
#define SKADOOSH_COMMON_H
#include <climits>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "rlights.h"

const bool DEBUG = true;
namespace dfo{
#define delta_time GetFrameTime()
#define GLSL_VERSION 330
    typedef int i16;
    typedef unsigned int u16;
#if UINT_MAX >= 4294967295U
    typedef int i32;
    typedef unsigned int u32;

#else
    typedef long i32;
        typedef unsigned long i32;
#endif
    extern u16 Id;
    const size_t named_size = 50;
    const size_t max_tags = 10;
}

#endif //SKADOOSH_COMMON_H
