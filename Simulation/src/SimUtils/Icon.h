#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <string>


struct Icon {
    int width = 0;
    int height = 0;
	GLuint id = -1;
};

struct Image {
    int width = 0;
    int height = 0;
    unsigned char* data = nullptr;

};

Icon Load(const std::string& name);
Image LoadPng(const std::string& image);
GLFWimage * LoadGLFWimage(const std::string& png);