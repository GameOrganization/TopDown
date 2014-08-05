#include "Entity.h"

#include "Vec2f.h"
#include <string>

Entity::Entity(){
    Entity("UNNAMED", Vec2f(0.0f, 0.0f), 0.0f, 0.0f, "");
}

Entity::Entity(std::string n, Vec2f p, float w, float h, std::string path){
    name = n;
    pos = p;
    width = w;
    height = h;
}

Entity::~Entity(){}

void Entity::update(float time){}

void Entity::setPos(float x, float y){
    pos = Vec2f(x, y);
}
Vec2f Entity::getPos(){
    return pos;
}

float Entity::getWidth(){
    return width;
}
float Entity::getHeight(){
    return height;
}

void Entity::setName(std::string newName){
    name = newName;
}
std::string Entity::getName(){
    return name;
}
