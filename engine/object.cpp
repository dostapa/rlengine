#include "object.h"
using namespace dfo;
u16 dfo::Id = 0;
object::object() {
    this->id=++Id;
    strcpy(this->name,"unnamed");
    for(int i = 0 ; i<max_tags; ++i){
        this->tags[i] = new char[named_size];
        strcpy(this->tags[i],"empty");
    }
    this->model = LoadModel("../models/default.obj");;
    this->num_of_tags = 0;
    this->tint = PURPLE;
    printf("<CONSTRUCTOR> Created new object #%d with name '%s'\n",
           this->id,this->name);
    this->collider = GetModelBoundingBox(this->model);

}

object::object(const char* nameIn, const char* modelPath, Color colorIn) {
    this->id=++Id;
    strcpy(this->name,nameIn);
    for(int i = 0 ; i<max_tags; ++i){
        this->tags[i] = new char[named_size];
        strcpy(this->tags[i],"empty");
    }
    this->model = LoadModel(modelPath);
    this->num_of_tags = 0;
    this->tint = colorIn;
    printf("<CONSTRUCTOR> Created new object #%d with name '%s', model path '%s'\n",
           this->id,this->name,modelPath);
    this->collider = GetModelBoundingBox(this->model);
}

object::object(const char* nameIn, char * tagsIn[], size_t tagsSizeIn, const char* modelPath, Color colorIn) {
    this->model = LoadModel(modelPath);
    this->id=++Id;
    strcpy(this->name,nameIn);
    for(int i = 0 ; i<max_tags; ++i){
        this->tags[i] = new char[named_size];
        if(i<tagsSizeIn)
            strcpy(this->tags[i],tagsIn[i]);
        else strcpy(this->tags[i],"empty");
    }
    this->tint = colorIn;
    this->num_of_tags = tagsSizeIn;
    printf("<CONSTRUCTOR> Created new object #%d with name '%s', model path '%s'\n",
           this->id,this->name,modelPath);
    this->collider = GetModelBoundingBox(this->model);
}

object &object::operator=(const object &other) {
    if(this == &other){
        return *this;
    }

    this->id = ++Id;

    //Free 'this' name
    delete[] this->name;
    this->name = nullptr;
    //Reallocate 'this' name and copy other name
    this->name = new char[named_size];
    strcpy(this->name,other.name);
    //Free 'this' tags
    for(int i = 0; i < num_of_tags; ++i){
        delete this->tags[i];
    }
    delete[] this->tags;
    //Reallocate 'this' tags and copy other tags
    this->tags = new char*[max_tags];
    for(int i = 0; i < this->num_of_tags; ++i){
        this->tags[i] = new char[named_size];
        if(i < other.num_of_tags)
            strcpy(this->tags[i],other.tags[i]);
        else
            strcpy(this->tags[i],"empty");
    }
    this->tint = other.tint;
    this->collider = other.collider;
    return *this;


}

object::~object() {
    printf("<DESTRUCTOR> Cleaning up object #%d with name '%s'\n",
           this->id,this->name);
    delete[] name;
    for(int i = 0; i < num_of_tags; ++i){
        delete tags[i];
    }
    delete[] tags;
    UnloadModel(this->model);
}



void object::replace_model(const char* fpIn) {
    UnloadModel(this->model);
    this->model = LoadModel(fpIn);

}

char *object::get_name() const {
    return this->name;
}

u16 object::get_id() const {
    return id;
}

Vector3 object::get_position(void) {
    return Vector3{this->model.transform.m12,this->model.transform.m13,this->model.transform.m14};
}

void object::set_position(Vector3 in) {
    this->model.transform.m12 = in.x;
    this->model.transform.m13 = in.y;
    this->model.transform.m14 = in.z;
    this->collider = GetModelBoundingBox(this->model);
}

void object::translate(Vector3 in){
    this->model.transform = MatrixMultiply(this->model.transform, MatrixTranslate(in.x,in.y,in.z));
    this->collider = GetModelBoundingBox(this->model);
}

void object::set_rotation(Vector4 in) {
    this->model.transform = MatrixMultiply(MatrixRotate({in.x,in.y,in.z},in.w), this->model.transform);
    this->collider = GetModelBoundingBox(this->model);

}

void object::set_scale(Vector3 in) {
    this->model.transform.m0 = in.x;
    this->model.transform.m5 = in.y;
    this->model.transform.m10 = in.z;
    this->collider = GetModelBoundingBox(this->model);
}

Vector3 object::get_scale(void) {
    return Vector3{this->model.transform.m0,this->model.transform.m5,this->model.transform.m10};
}



void object::draw_object() {
    DrawModelEx(this->model, Vector3Zero(), Vector3Zero(), 0, {1,1,1}, this->tint);
    DrawBoundingBox(this->collider, GREEN);
    set_position(this->get_position());
    set_rotation({0,0,0,0});
    set_scale(this->get_scale());
}
