#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include "GL/ShaderProgram.h"

GLuint loadShader(const char* filename, GLenum type) {
    printf("Loading shader: %s\n", filename);
    //open the file and calculate the length of the file
    std::ifstream file;
    file.open(filename, std::ios::in);
    if (!file || !file.good()) {
        printf("Error opening file\n");
        return 0;
    }
    file.seekg(0, std::ios::end);
    std::size_t len = file.tellg();
    file.seekg(0, std::ios::beg);

    //read the data from the file into a buffer
    char* src = (char*)malloc(len + 1);
    file.read(src, len);
    file.close();
    src[len] = '\0';

    //create and compile the shader
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const char**)&src, NULL);
    glCompileShader(id);
    free(src);

    GLint compiled;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint error_len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &error_len);
        char* error = (char*)malloc(error_len);
        glGetShaderInfoLog(id, error_len, NULL, error);
        printf(error);
        free(error);
    }

    return id;
}

ShaderProgram::ShaderProgram(const char* vert, const char* frag) {
    GLuint vertID = loadShader(vert, GL_VERTEX_SHADER);
    GLuint fragID = loadShader(frag, GL_FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id, vertID);
    glAttachShader(id, fragID);
    glLinkProgram(id);

    GLint linked;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint error_len;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &error_len);
        char* error = (char*)malloc(error_len);
        glGetProgramInfoLog(id, error_len, NULL, error);
        printf(error);
        free(error);
    }

    glDeleteShader(vertID);
    glDeleteShader(fragID);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

void ShaderProgram::use() {
    glUseProgram(id);
}

GLuint ShaderProgram::getUniform(const char* name) {
    return glGetUniformLocation(id, name);
}
