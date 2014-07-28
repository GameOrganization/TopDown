#include "GLUtil.h"

#include "GL/glew.h"

GLuint vertexArray = 0;

void GLUtil::initGL(float w, float h) {
    //Initialize the vertex array
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);



}
