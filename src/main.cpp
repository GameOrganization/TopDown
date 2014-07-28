#include <iostream>
#include <stdlib.h>
#include "Window.h"
#include "GLUtil.h"

#define VSYNC 1

void init();
void update(float time);
void draw();

float w = 0.0f, h = 0.0f;

GLuint vertexBuffer;

GLuint programID = GLUtil::loadShaderProgram("vertexShader.txt", "fragmentShader.txt");

void update(float time) {}

static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

    //close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void mouseHandler(GLFWwindow* window, int button, int action, int mods) {}

int main(int argc, char *argv[]) {

    //Attempt to create window
    if (Window::create()) {
        Window::destroy();
        return 1;
    }

    //Initialize world
    init();

    //Loop updates world, draws it, and then shows it on the screen
    while (Window::isOpen()) {
        update((float)Window::getFrameTime());
        draw();
        Window::update();
    }

    //Close the window
    Window::destroy();
    return 0;
}

void init() {
    //Set the key handler callback
    glfwSetKeyCallback(Window::ptr, keyHandler);
    glfwSetMouseButtonCallback(Window::ptr, mouseHandler);

    //Enable vsync so mike's computer doesn't shit itself
    //just noticed this and laughed in the office i dont care i love it
    glfwSwapInterval(VSYNC);

    w = 10.0f * (float)Window::width() / (float)Window::height();
    h = 10.0f;

    //glViewport(0.0f, 0.0f, Window::width(), Window::height());
    glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    glOrtho(-w/2, w/2, 0.0f, h, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    static const GLfloat triangle[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    };

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}
