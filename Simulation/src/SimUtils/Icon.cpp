#include "Icon.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Icon Load(const std::string& name) {
    int image_width, image_height;
    unsigned char* image_data = stbi_load(name.c_str(), &image_width, &image_height, NULL, 4);

    // Turn the RGBA pixel data into an OpenGL texture:
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    return { image_width, image_height, texture };
}