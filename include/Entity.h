 #ifndef ENTITY_H
#define ENTITY_H
#include "Vec2f.h"
#include <string>

class Entity{
    //current world coordinates (x,y)
    Vec2f pos;
    //dimensions of the bounding box
    float width,height;
    //name of the thing
    std::string name;
    //filepath for the texture
    std::string texpath;
    public:
        Entity();
        Entity(std::string n, Vec2f p, float w, float h, std::string path);
        virtual ~Entity();

        //called in main loop
        void update(float time);

        void setPos(float x, float y);
        Vec2f getPos();

        float getWidth();
        float getHeight();

        void setName(std::string newName);
        std::string getName();

    protected:
    private:
};

#endif // ENTITY_H
