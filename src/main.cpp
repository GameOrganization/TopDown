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

#define VSYNC 1

void init();
void update(float time);
void draw();

float w = 0.0f, h = 0.0f, fov = 45.0f, speed = 3.0f;

glm::vec3 cameraPos = glm::vec3(0,0,5);
glm::vec3 cameraFront = glm::vec3(0,0,1);
glm::vec3 cameraRight = glm::vec3(1,0,0);
glm::vec3 cameraUp = glm::vec3(0,1,0);

GLuint vertexArray, vertexBuffer, colorBuffer;

GLuint programID;

//a cube, centered at the origin, sides of length 2, represented by 12 triangles
static const GLfloat cube[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f
};

void update(float time) {
    int err;
    if (err = glGetError()) {
        std::cout << "ERROR: " << gluErrorString(err) << std::endl;
    }
    /*  glm::mat4 projection = glm::perspective(fov, (float)w/h, 0.1f, 100.0f);   */
    glm::mat4 projection = glm::ortho(-2*w/h, 2*w/h, -2.0f, 2.0f, -2.0f, 2.0f);

    glm::mat4 view = glm::lookAt(
        cameraPos,                  //camera position
        cameraPos + cameraFront,    //camera target
        cameraUp);                  //vector pointing up *
    //glm::mat4 view = glm::mat4(1.0f);

    //identity matrix (since the cube is at the origin
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    //model = glm::rotate(model, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mvp = projection * view * model;

    std::cout << "get uniform" << std::endl;
    GLuint matrixID = glGetUniformLocation(programID, "MVP");
    update(0.0f);

    std::cout << "set uniform" << std::endl;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(mvp));
    update(0.0f);


}

static void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

    //close window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        std::cout<<"CLOSING"<<std::endl;
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS){
        std::cout<<"W PRESSED"<<std::endl;
        cameraPos += cameraFront * (float)Window::getFrameTime() * speed;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS){
        std::cout<<"S PRESSED"<<std::endl;
        cameraPos -= cameraFront * (float)Window::getFrameTime() * speed;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        std::cout<<"D PRESSED"<<std::endl;
        cameraPos += cameraRight * (float)Window::getFrameTime() * speed;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS){
        std::cout<<"A PRESSED"<<std::endl;
        cameraPos += cameraRight * (float)Window::getFrameTime() * speed;
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

    update(0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::cout << "vertex array" << std::endl;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    update(0.0f);

    //generating and binding the vertex buffer
    std::cout << "binding vertex buffers" << std::endl;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    update(0.0f);

    //generating and binding the color buffer, using the vertices positions as rgb values
    std::cout << "binding color buffers" << std::endl;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    update(0.0f);

    //setting the 'clear' color (color of the background after clearing it)
    std::cout << "clearcolor" << std::endl;
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    update(0.0f);

    //loading the vertex and fragment shaders
    std::cout << "load shaders" << std::endl;
    programID = GLUtil::loadShaderProgram("vertexShader.txt", "fragmentShader.txt");
    std::cout << programID << std::endl;
    glUseProgram(programID);
    update(0.0f);
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
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
    glDisableVertexAttribArray(0);
}
