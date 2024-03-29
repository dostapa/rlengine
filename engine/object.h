//
// Created by soup99 on 2024-03-26.
//
#include "common.h"
#ifndef SKADOOSH_OBJECT_H
#define SKADOOSH_OBJECT_H

namespace dfo {
    class object {
    private:
        u16 id;

        char *name = new char[named_size];
        char **tags = new char *[max_tags];

        size_t num_of_tags;

        Model model{};
        BoundingBox collider{};
        Color tint;
    public:
        object();

        object(const char *, const char *, Color);

        object(const char *, char *[], size_t, const char *, Color);

        object &operator=(const object &other);

        inline BoundingBox& get_collider() {
            return collider;
        }
        ~object();


        void replace_model(const char *fpIn);

        [[nodiscard]] u16 get_id() const;

        [[nodiscard]] char *get_name() const;

        Vector3 get_position(void);

        Vector3 get_scale(void);

        void set_position(Vector3);

        void set_rotation(Vector4);

        void set_scale(Vector3);

        void translate(Vector3);

        void draw_object();


    };
}

#endif //SKADOOSH_OBJECT_H