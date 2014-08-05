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

Entity::void update(float time);

Entity::void setPos(float x, float y){
    pos = Vec2f(x, y);
}
Entity::Vec2f getPos(){
    return pos;
}

Entity::float getWidth(){
    return width;
}
Entity::float getHeight(){
    return height;
}

Entity::void setName(std::string newName){
    name = newName;
}
Entity::std::string getName(){
    return name;
}
