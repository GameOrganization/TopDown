#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GL/glew.h"
#include "GL/glfw3.h"

class ShaderProgram {
    GLuint id;
public:
    ShaderProgram(const char* vert, const char* frag);
    virtual ~ShaderProgram();

    void use();
    GLuint getUniform(const char* name);
};

#endif // SHADERPROGRAM_H
