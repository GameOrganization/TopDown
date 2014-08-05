#include "GLUtil.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "lodepng.h"

//GLuint vertexArray = 0;
/*
void GLUtil::initGL(float w, float h) {
    //Initialize the vertex array
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);



}
*/

GLuint GLUtil::loadTexture(const char* filename) {
    GLuint tex = 0;
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename);
    if(error) {
        std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 0;
    }

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    return tex;
}

GLuint GLUtil::loadShaderProgram(const char* vertFile, const char* fragFile){

    //ID's for the two shaders
    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //reading in the vertex shader code
    std::string VertexShaderSource;
    std::ifstream VertexShaderStream(vertFile, std::ios::in);

    if(VertexShaderStream.is_open()){
        std::string line="";
        while(getline(VertexShaderStream, line)){
            VertexShaderSource+="\n" + line;
        }
        VertexShaderStream.close();
    }

    //reading in the fragment shader code
    std::string FragmentShaderSource;
    std::ifstream FragmentShaderStream(fragFile, std::ios::in);

    if(FragmentShaderStream.is_open()){
        std::string line="";
        while(getline(FragmentShaderStream, line)){
            FragmentShaderSource+="\n" + line;
        }
        FragmentShaderStream.close();
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    //compiling the vertex shader
    std::cout<<"Compiling vertex shader at: "<<vertFile<<std::endl;
    char const* VertexShaderPointer = VertexShaderSource.c_str();
    glShaderSource(VertexShader, 1, &VertexShaderPointer, NULL);
    glCompileShader(VertexShader);

    //generating error message for vertex shader
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(VertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    char* VertexShaderMessage = (char*)malloc(infoLogLength);
    glGetShaderInfoLog(VertexShader, infoLogLength, NULL, VertexShaderMessage);
    std::cout<<VertexShaderMessage<<std::endl;
    free(VertexShaderMessage);

    //compiling the fragment shader
    std::cout<<"Compiling fragment shader at: "<<fragFile<<std::endl;
    char const* FragmentShaderPointer = FragmentShaderSource.c_str();
    glShaderSource(FragmentShader, 1, &FragmentShaderPointer, NULL);
    glCompileShader(FragmentShader);

    //generating error message for fragment shader
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(FragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    char* FragmentShaderMessage = (char*)malloc(infoLogLength);
    glGetShaderInfoLog(FragmentShader, infoLogLength, NULL, FragmentShaderMessage);
    std::cout<<FragmentShaderMessage<<std::endl;

    //link the two shaders with the program
    std::cout<<"Linking"<<std::endl;
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShader);
    glAttachShader(ProgramID, FragmentShader);
    glLinkProgram(ProgramID);

    //generating error message for the program
    glGetShaderiv(ProgramID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
    char* ProgramMessage = (char*)malloc(infoLogLength);
    glGetProgramInfoLog(ProgramID, infoLogLength, NULL, ProgramMessage);
    std::cout<<ProgramMessage<<std::endl;
    free(ProgramMessage);

    //delete the shaders
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    return ProgramID;
}
