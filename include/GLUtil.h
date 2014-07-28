#ifndef GLUTIL_H
#define GLUTIL_H


namespace GLUtil {

void initGL(float w, float h);

GLuint loadShaderProgram(const char* vertFile, const char* fragFile);

} //namespace GLUtil

#endif // GLUTIL_H
