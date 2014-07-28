#include "Window.h"
#include <iostream>

//Initialize to NULL to signify that there is no window
GLFWwindow* Window::ptr = NULL;

//Data structure to hold the screen resolution, not directly accessible through the api
struct res {
    int w;
    int h;
} resolution = {0, 0};

//Data structure to count fps and frame time
struct fc {
    int frames;
    double time;
    int fps;
    double prev, curr;
} fps = {0, 0.0, 0, 0.0, 0.0};

//Callback function to display error codes and descriptions from GLFW
static void error(int error, const char* description) {
    std::cerr << "Error " << error << ": " << description << std::endl;
}

int Window::create() {
    //Set the error callback for GLFW
    glfwSetErrorCallback(error);

    //If the window already exists, dont overwrite
    if (ptr)
        return 1;

    //If GLFW fails to initialize, return 1
    if (!glfwInit())
        return 1;

    //Set up the window with compatibility for OpenGL 3.3
    glfwWindowHint(GLFW_SAMPLES, 4); //Enable 4x antialiasing because yes
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Needed on MacOS for some reason
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Use core profile (idk what that means)

    //Attempt to create the fullscreen window with native resolution
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* display = glfwGetVideoMode(monitor);
    ptr = glfwCreateWindow(display->width, display->height, WINDOW_TITLE, monitor, NULL);

    //If window is still null, return 1
    if (!ptr)
        return 1;

    //Update the resolution struct
    resolution.w = display->width;
    resolution.h = display->height;

    //Make the window the current OpenGL context
    glfwMakeContextCurrent(ptr);

    //Attempt to initialize GLEW, if an error occurred, return null
    glewExperimental = true; //needed for core profile
    if (glewInit() != GLEW_OK)
        return 1;

    //Initialize the fps counter
    fps.curr = glfwGetTime();

    //Return 0 since no error occurred
    return 0;
}

void Window::destroy() {
    //Destroy window
    glfwDestroyWindow(ptr);
    //Reflect that the window has been destroyed
    ptr = NULL;
    resolution.w = 0;
    resolution.h = 0;
    //Terminate GLFW
    glfwTerminate();
}

int Window::width() {
    return resolution.w;
}

int Window::height() {
    return resolution.h;
}

bool Window::isOpen() {
    return !glfwWindowShouldClose(ptr);
}

void Window::update() {
    glfwSwapBuffers(ptr);
    glfwPollEvents();

    fps.prev = fps.curr;
    fps.curr = glfwGetTime();
    fps.frames++;
    fps.time += (fps.curr - fps.prev);
    if (fps.time >= 1.0) {
        fps.fps = fps.frames;
        fps.frames = 0;
        fps.time -= 1.0;
        std::cout << fps.fps << "fps" << std::endl;
    }
}

double Window::getFrameTime() {
    return (fps.curr - fps.prev);
}

int Window::getFPS() {
    return fps.fps;
}
