//
// Created by soup99 on 2024-03-27.
//

#ifndef SKADOOSH_SCENE_H
#define SKADOOSH_SCENE_H
#define MAXENTITIES 200
#include "common.h"
#include "object.h"
namespace dfo {
    class scene {
    private:
        std::vector<object *> s;
    public:
        scene();

        ~scene() = default;

        void add_to_scene(const char *, const char *, Color);

        object *add_to_scene(const char *, const char *, char *[], size_t, Color);

        void clear_scene();

        object *get_at(u16 idx);

        object *get_by_id(u16 id);

        object *get_all_by_name(const char *name);

        bool remove_by_id(u16 id);

        void remove_all_by_name(const char *name);

        void draw_scene();
    };

}
#endif //SKADOOSH_SCENE_H
