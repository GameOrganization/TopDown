#ifndef CAMERA_H
#define CAMERA_H


class camera
{
    public:
        camera();
        virtual ~camera();
        float x, y;
        void moveCamera(float mouseX, float mouseY, float deltaT, int key, int scancode, int action, int mods);
    protected:
    private:
};

#endif // CAMERA_H
