cmake_minimum_required(VERSION 3.27)
project(rlgame)

set(CMAKE_CXX_STANDARD 17)
find_library(raylib raylib/lib DIRECTORY)

if(WIN32)
    link_libraries(raylib opengl32 gdi32 winmm)
endif(WIN32)

if(UNIX)
    link_libraries(raylib opengl32 gdi32 winmm)
endif(UNIX)

add_executable(rlgame
        src/main.cpp
        engine/object.cpp
        engine/object.h
        engine/scene.cpp
        engine/scene.h
        engine/util.cpp
        engine/util.h
)
