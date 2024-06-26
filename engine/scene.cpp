//
// Created by soup99 on 2024-03-27.
//

#include "scene.h"
#include <cstring>
using namespace dfo;
object * scene::add_to_scene(const char* name_in, Model* mdl_in, Color tint_in){
    return s.emplace_back(new object(name_in, mdl_in, tint_in));
}
object * scene::add_to_scene(const char* nameIn, const char* modelPath, Color tintIn) {
    return s.emplace_back(new object(nameIn,modelPath, tintIn));
}

object * scene::add_to_scene(const char *nameIn, const char *modelPath, char *tags[], size_t num_of_tags, Color tintIn) {
    return s.emplace_back(new object(nameIn,tags,num_of_tags,modelPath, tintIn));
}

object *scene::get_by_id(u16 id) {
    for(auto& obj : s){
        if(obj->get_id() == id) return obj;
    }
    return nullptr;
}

object *scene::get_all_by_name(const char *name) {
    u16 counter = 0,j=0;
    for(auto& obj : s){
        if(strcmp(name, obj->get_name()) != 0){
            counter++;
        }
    }
    auto* objects = new object[counter];

    for(auto& obj : s){
        if(strcmp(name, obj->get_name()) != 0) {
            objects[j++] = *obj;
        }
    }
    return objects;
}
void scene::clear_scene(){
    while(s.size() != 0){
        delete s[0];
        s.erase(s.begin());
    }
}


object *scene::get_at(u16 idx) {
    auto* object = s.at(idx);
    return object;
}

bool scene::remove_by_id(u16 id) {
    for(u16 i = 0 ; i < s.size() ; i++){
        u16 currId = s.at(i)->get_id();
        if(currId == id){
            delete s.at(i);
            s.erase(s.begin()+i);
            return true;
        }
    }
    return false;
}

scene::scene() = default;

void scene::draw_scene() {
    for(auto& obj : s){
        if (obj->draw)
            obj->draw_object();
        if (obj->border)
            DrawBoundingBox(obj->get_collider(), GREEN);
    }

}

size_t scene::get_size() {
    return s.size();
}

object *scene::add_to_scene(object* in) {
    s.push_back(in);
    return s.back();
}
