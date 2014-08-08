#include <stdio.h>
#include "GL/lodepng.h"
#include "GL/Texture.h"

Texture::Texture(const char* filename) : id(0) {
    std::vector<unsigned char> data;
    unsigned error = lodepng::decode(data, width, height, filename);
    if (error) {
        printf("Decoder error %d: %s\n", error, lodepng_error_text(error));
    } else {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    }
}

Texture::Texture(const char* filename, GLenum minFilter, GLenum maxFilter) : id(0) {
    std::vector<unsigned char> data;
    unsigned error = lodepng::decode(data, width, height, filename);
    if (error) {
        printf("Decoder error %d: %s\n", error, lodepng_error_text(error));
    } else {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    }
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void Texture::bind(unsigned texture) {
    glActiveTexture(GL_TEXTURE0 + texture);
    glBindTexture(GL_TEXTURE_2D, id);
}

unsigned int Texture::getWidth() {
    return width;
}

unsigned int Texture::getHeight() {
    return height;
}
