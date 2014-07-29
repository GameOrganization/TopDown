#include <iostream>
#include <stdlib.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#define GLFW_INCLUDE_GLU
#include "Window.h"
#include "GLUtil.h"

#define VSYNC 1

void init();
void update(float time);
void draw();

float w = 0.0f, h = 0.0f, fov = 45.0f;

GLuint vertexBuffer;

GLuint programID;

void update(float time) {
    int err;
    if (err = glGetError()) {
        std::cout << "ERROR: " << gluErrorString(err) << std::endl;
    }
}

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

    static const GLfloat triangle[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    };

    //generating and binding the vertex buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    //setting the 'clear' color (color of the background after clearing it)
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

    //loading the vertex and fragment shaders
    programID = GLUtil::loadShaderProgram("vertexShader.txt", "fragmentShader.txt");

    glm::mat4 projection = glm::perspective(fov, (float)w/h, 1.0f, 100.0f);

    glm::mat4 view = glm::lookAt(
        glm::vec3(4.0f, 3.0f, 3.0f),   //camera position
        glm::vec3(0.0f, 0.0f, 0.0f),   //camera target
        glm::vec3(0.0f,-1.0f, 0.0f));  //vector pointing up

    //identity matrix (since the triangle is at the origin
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 cumulativeTransformMatrix = projection * view * model;

    GLuint matrixID = glGetUniformLocation(programID, "cumulativeTransformMatrix");

    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &cumulativeTransformMatrix[0][0]);
}

void draw(){

    //clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //telling gl to use our shaders
    glUseProgram(programID);

    //loading the vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}
