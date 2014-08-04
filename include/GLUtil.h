#include <GL/glew.h>
#include <GL/wglew.h>

#ifndef GLUTIL_H
#define GLUTIL_H


namespace GLUtil {

void initGL(float w, float h);

// TODO replace these sorts of things with classes
GLuint loadTexture(const char* filename);
GLuint loadShaderProgram(const char* vertFile, const char* fragFile);

} //namespace GLUtil

#endif // GLUTIL_H
