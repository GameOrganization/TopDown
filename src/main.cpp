#include <iostream>
#include <stdlib.h>
#define GLM_FORCE_RADIANS //suppresses errors about method deprecation, purely cosmetic
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLFW_INCLUDE_GLU
#include "Window.h"
#include "GLUtil.h"
#include "Vec2f.h"

#define VSYNC 0

void init();
void update(float time);
void draw();

float w = 0.0f, h = 0.0f, fov = 45.0f, speed = 3.0f;

GLuint vertexArray, vertexBuffer, colorBuffer;

Vec2f camera;

GLuint programID, mvpID;

//changed it from a cube to a plane, since that's what we'll be using most likely
static const GLfloat plane[] = {
     1.0f,  1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
};

void update(float time) {
    int err = glGetError();
    if (err) {
        std::cout << "ERROR: " << gluErrorString(err) << std::endl;
    }

    //keep the cursor in the center of the screen
    glfwSetCursorPos(Window::ptr, (double)w/2, (double)h/2);

    glm::mat4 projection = glm::ortho(-2*w/h, 2*w/h, -2.0f, 2.0f, -2.0f, 2.0f);

    Vec2f motion(0.0f, 0.0f);
    if (glfwGetKey(Window::ptr, GLFW_KEY_W) == GLFW_PRESS) {
        motion.y -= 1.0f;
    }
    if (glfwGetKey(Window::ptr, GLFW_KEY_A) == GLFW_PRESS) {
        motion.x += 1.0f;
    }
    if (glfwGetKey(Window::ptr, GLFW_KEY_S) == GLFW_PRESS) {
        motion.y += 1.0f;
    }
    if (glfwGetKey(Window::ptr, GLFW_KEY_D) == GLFW_PRESS) {
        motion.x -= 1.0f;
    }

    if (motion.lengthSquared() > 0) {
        motion.normalize();
        camera += motion * time;
    }

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(camera.x, camera.y, 0.0f));

    //model = glm::rotate(model, 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 mvp = projection * view * model;

    glUniformMatrix4fv(mvpID, 1, GL_FALSE, glm::value_ptr(mvp));
}

static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

    //close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        std::cout<<"CLOSING"<<std::endl;
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
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

    w = Window::width();
    h = Window::height();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //hides the cursor
    glfwSetInputMode(Window::ptr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    //generating and binding the vertex buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

    //generating and binding the color buffer, using the vertices positions as rgb values
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

    //setting the 'clear' color (color of the background after clearing it)
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

    //loading the vertex and fragment shaders
    programID = GLUtil::loadShaderProgram("vertexShader.txt", "fragmentShader.txt");
    std::cout << programID << std::endl;
    glUseProgram(programID);
    mvpID = glGetUniformLocation(programID, "MVP");

}

void draw(){

    //clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //loading the vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //loading the colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //6 faces, 2 triangles per face, 3 vertices per triangle
    glDrawArrays(GL_TRIANGLES, 0, sizeof(plane));
    glDisableVertexAttribArray(0);
}
