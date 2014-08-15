#include "Window.h"
#include <stdio.h>

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
static void error(int error, const char* desc) {
    printf("Error %d: %s\n", error, desc);
}

static void printGLInfo() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); //TODO: find why core profile isn't working

    //Attempt to create the fullscreen window with native resolution
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* display = glfwGetVideoMode(monitor);
    ptr = glfwCreateWindow(display->width/2, display->height/2, WINDOW_TITLE, NULL, NULL);
    //TODO: decide whether to do fullscreen or ust

    //If window is still null, return 1
    if (!ptr)
        return 1;
    //Update the resolution struct
    resolution.w = display->width/2;
    resolution.h = display->height/2;

    //Make the window the current OpenGL context
    glfwMakeContextCurrent(ptr);

    //Attempt to initialize GLEW, if an error occurred, return null
    glewExperimental = true; //needed for core profile
    if (glewInit() != GLEW_OK)
        return 1;

    //Initialize the fps counter
    fps.curr = glfwGetTime();

    //Print debug information
    printGLInfo();

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
        printf("%dfps\n", fps.fps);
    }
}

double Window::getFrameTime() {
    return (fps.curr - fps.prev);
}

int Window::getFPS() {
    return fps.fps;
}
