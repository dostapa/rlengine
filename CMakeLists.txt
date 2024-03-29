cmake_minimum_required(VERSION 3.27)
project(skadoosh)

set(CMAKE_CXX_STANDARD 23)
link_libraries(raylib opengl32 gdi32 winmm)
add_executable(skadoosh main.cpp
        engine/object.h
        engine/object.cpp
        engine/scene.cpp
        engine/scene.h
        engine/util.cpp
        engine/util.h
        engine/common.h)
